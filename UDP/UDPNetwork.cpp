#include "UDPNetwork.h"
#include "../Proto/command.pb.h"
#include "NetworkPackage.h"
#include "iostream"
#include "../ServerFrame.h"

AUDPNetwork::AUDPNetwork()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SenderSocket = NULL;

	Receiver = NULL;

	ShowOnScreenDebugMessages = true;


	GOOGLE_PROTOBUF_VERIFY_VERSION;

}

// Called when the game starts or when spawned
void AUDPNetwork::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AUDPNetwork::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUDPNetwork::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	delete Receiver;
	Receiver = NULL;

	if (SenderSocket) //Clear all sockets!
	{
		SenderSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(SenderSocket);
	}
}

bool AUDPNetwork::RamaUDPSender_SendString(FString ToSend)
{
	if (!SenderSocket)
	{
		ScreenMsg("No sender socket");
		return false;
	}
	int32 BytesSent = 0;
	FString serialized = ToSend;
	TCHAR* serializedChar = serialized.GetCharArray().GetData();
	int32 size = FCString::Strlen(serializedChar);
	int32 sent = 0;
	//SenderSocket->SendTo(Writer.GetData(), Writer.Num(), BytesSent, *RemoteAddr);
	SenderSocket->SendTo((uint8*)TCHAR_TO_UTF8(serializedChar), size, BytesSent, *RemoteAddr); //���͸�Զ�˵�ַ

	if (BytesSent <= 0)
	{
		const FString Str = "Socket is valid but the receiver received 0 bytes, make sure it is listening properly!";
		UE_LOG(LogTemp, Error, TEXT("%s"), *Str);
		ScreenMsg(Str);
		return false;
	}

	ScreenMsg("UDP Send Succcess! INFO Sent = ", ToSend);
	UE_LOG(LogTemp, Warning, TEXT("hahahaha"));

	return true;
}

bool AUDPNetwork::SendPackage(const FNetworkPackage& package)
{
	std::string str = package.byteString;
	RouteMsg msg = RouteMsg();
	switch (package.type)
	{
	case EPackageType::T_REGISTER:
		msg.set_type(RouteMsg_Type_REGISTER);
		break;
	case EPackageType::T_CLIENT_FRAME:
		msg.set_type(RouteMsg_Type_CLIENT_FRAME);
		break;
	case EPackageType::T_CLIENT_SYNC_POSITION:
		msg.set_type(RouteMsg_Type_CLIENT_POSITION_SYNC);
		break;
	}
	msg.set_msg(str);

	std::string out;
	msg.SerializeToString(&out);

	const uint8* buffer = (const uint8*)(out.c_str());

	int32 BytesSent = 0;
	int32 size = out.length();

	if (SenderSocket->SendTo(buffer, size, BytesSent, *RemoteAddr))
	{
		return true;
	}
	else
		return false;
}

bool AUDPNetwork::SendFrame(const FClientFrame& frame)
{
	FrameToMsg(frame);

	cacheMsg->Clear();

	cacheMsg->set_type(RouteMsg_Type::RouteMsg_Type_CLIENT_FRAME);

	std::string frame_buffer;

	cacheFrame->SerializeToString(&frame_buffer);

	cacheMsg->set_msg(frame_buffer);

	std::string out;
	cacheMsg->SerializeToString(&out);

	const uint8* buffer = (const uint8*)(out.c_str());

	int32 BytesSent = 0;
	int32 size = out.length();

	if (SenderSocket->SendTo(buffer, size, BytesSent, *RemoteAddr))
	{

		return true;
	}
	else
		return false;
}

bool AUDPNetwork::StartUDPSender(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort, bool UDP)
{
	cacheFrame = google::protobuf::Arena::CreateMessage<ClientFrame>(&arena);
	cacheInput = google::protobuf::Arena::CreateMessage<CameraAxisInput>(&arena);
	cacheMsg = google::protobuf::Arena::CreateMessage<RouteMsg>(&arena);
	cachePosition = google::protobuf::Arena::CreateMessage<PositionData>(&arena);
	cacheSync = google::protobuf::Arena::CreateMessage<ClientPositionSync>(&arena);


	RemoteAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	bool bIsValid;
	RemoteAddr->SetIp(*TheIP, bIsValid);
	RemoteAddr->SetPort(ThePort);

	if (!bIsValid)
	{
		ScreenMsg("Rama UDP Sender>> IP address was not valid!", TheIP);
		return false;
	}

	SenderSocket = FUdpSocketBuilder(*YourChosenSocketName)
		.AsReusable()
		.WithBroadcast()
		.WithSendBufferSize(2 * 1024 * 1024);

	FTimespan timeSpan = FTimespan::FromMilliseconds(10);
	Receiver = new FUdpSocketReceiver(SenderSocket, timeSpan, TEXT("Receiver"));
	Receiver->SetMaxReadBufferSize(512);
	Receiver->OnDataReceived().BindUObject(this, &AUDPNetwork::OnUdpDataReady);
	Receiver->Start();

	//Set Send Buffer Size
	int32 SendSize = 2 * 1024 * 1024;
	SenderSocket->SetSendBufferSize(SendSize, SendSize);
	SenderSocket->SetReceiveBufferSize(SendSize, SendSize);
	if (bIsValid)
	{
		bIsValid = true;
	}

	return bIsValid;
}

void AUDPNetwork::OnUdpDataReady(const FArrayReaderPtr& ArrayReader, const FIPv4Endpoint& Endpoint)
{
	cacheMsg->Clear();
	cacheMsg->ParseFromArray(ArrayReader->GetData(), ArrayReader->Num());

	RouteMsg_Type msg_type = cacheMsg->type();
	std::string msg_buffer = cacheMsg->msg();

	if (msg_type == ::RouteMsg_Type::RouteMsg_Type_REGISTER_RESPONSE)
	{
		ScreenMsg(TEXT("收到注册回复"));
		RegisterResponse response;
		response.ParseFromString(msg_buffer);

		bool result = response.result();
		if (result)
		{
			int32 id = response.id();
			int32 port = response.port();
			int32 frame_id = response.serverframe();
			FString reason(FString(response.reason().c_str()));

			TArray<FPlayerInfo> players;
			for (int i = 0; i < response.playerlist_size(); i++)
			{
				PlayerInfo info = response.playerlist(i);
				FPlayerInfo f_info;
				f_info.Id = info.id();
				f_info.Title = FString(info.title().c_str());
				players.Add(f_info);
			}
			FServerResigterResponse res;
			res.result = result;
			res.id = id;
			res.port = port;
			res.frameId = frame_id;
			res.players = players;
			OnRegisterResponse.Broadcast(res);
		}

		return;
	}


	if (msg_type == ::RouteMsg_Type::RouteMsg_Type_SERVER_FRAME)
	{
		ServerFramePackage serverPackage;
		serverPackage.ParseFromString(msg_buffer);

		/*ServerFrame f = serverPackage.current();
		for (int i = 0; i < f.clientinputs_size(); i++)
		{
			ClientFrame c_frame = f.clientinputs(i);
			if (c_frame.clientid() == 1)
			{
				CameraAxisInput _input = c_frame.input();
				ScreenMsg(
					TEXT("camera=") + FString::SanitizeFloat(_input.camerayaw()) +
					TEXT(", moveForward=") + FString::SanitizeFloat(_input.moveforward()) +
					TEXT(", moveRight=") + FString::SanitizeFloat(_input.moveright())
				);
			}
		}*/

		FServerFramePackage package;
		package = FServerFramePackage(&serverPackage);
		OnServerFrame.Broadcast(package);
		return;
	}

	if (msg_type == ::RouteMsg_Type::RouteMsg_Type_CLIENT_POSITION_SYNC)
	{
		ScreenMsg(TEXT("接收到位置同步帧！"));
		ClientPositionSync syncData;
		syncData.ParseFromString(msg_buffer);
		PositionData posData = syncData.position();
		FClientPositionSync sync_out;
		sync_out.clientId = syncData.clientid();
		sync_out.position = FVector(posData.x(), posData.y(), posData.z());
		OnPositionSync.Broadcast(sync_out);
		return;
	}

	if (msg_type == ::RouteMsg_Type::RouteMsg_Type_PLAYER_ENTER)
	{
		ScreenMsg(TEXT("客人进入房间！"));
		PlayerInfo data;
		data.ParseFromString(msg_buffer);
		FPlayerInfo info;
		info.Id = data.id();
		info.Title = FString(data.title().c_str());
		OnPlayerEnter.Broadcast(info);
		return;
	}

	ScreenMsg(TEXT("未能识别类型！"));
	return;
}

void AUDPNetwork::GenerateRegisterPackage(const FString& name, FNetworkPackage& package)
{
	std::string str(TCHAR_TO_UTF8(*name));
	RegisterMsg registerMsg;
	registerMsg.set_title(str);
	std::string out;
	registerMsg.SerializeToString(&out);
	package = FNetworkPackage();
	package.type = EPackageType::T_REGISTER;
	package.byteString = out;
}

void AUDPNetwork::GenerateSyncPackage(const FClientPositionSync& syncData, FNetworkPackage& package)
{
	cacheSync->Clear();
	cacheSync->set_clientid(syncData.clientId);
	cachePosition->set_x(syncData.position.X);
	cachePosition->set_y(syncData.position.Y);
	cachePosition->set_z(syncData.position.Z);
	cacheSync->set_allocated_position(cachePosition);

	std::string out;
	cacheSync->SerializeToString(&out);
	package = FNetworkPackage();
	package.type = EPackageType::T_CLIENT_SYNC_POSITION;
	package.byteString = out;
}

ClientFrame* AUDPNetwork::cacheFrame;
CameraAxisInput* AUDPNetwork::cacheInput;
ClientPositionSync* AUDPNetwork::cacheSync;
PositionData* AUDPNetwork::cachePosition;
google::protobuf::Arena AUDPNetwork::arena;

void AUDPNetwork::GenerateClientFramePackage(const FClientFrame& frame, FNetworkPackage& package)
{
	cacheFrame->Clear();
	FrameToMsg(frame);
	std::string frame_out_buffer;
	cacheFrame->SerializeToString(&frame_out_buffer);

	//package组织
	package = FNetworkPackage();
	package.type = EPackageType::T_CLIENT_FRAME;
	package.byteString = frame_out_buffer;
}

void AUDPNetwork::InputToMsg(FThirdPlayerInput thirdInput) {
	cacheInput->Clear();
	cacheInput->set_camerayaw(thirdInput.cameraYaw);
	cacheInput->set_moveright(thirdInput.moveRight);
	cacheInput->set_moveforward(thirdInput.moveForward);
	cacheInput->set_jump(thirdInput.jump);
}

void AUDPNetwork::FrameToMsg(FClientFrame clientFrame) {
	cacheFrame->set_clientid(clientFrame.clientId);
	InputToMsg(clientFrame.input);
	cacheFrame->set_allocated_input(cacheInput);
}
