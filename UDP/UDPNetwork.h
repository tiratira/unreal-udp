

#pragma once
#include "Engine.h"
#include <iostream>
#include "../ServerFrame.h"
#include "Networking.h"
#include "../UDP/NetworkPackage.h"
#include "GameFramework/Actor.h"
#include "UDPNetwork.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRegisterResponseDelegate,
const FServerResigterResponse&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerFrameDelegate, const FServerFramePackage&, ServerFramePackage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUDPMessageDelegate, const FString&, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPositionSyncDelegate, const FClientPositionSync&, SyncData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerEnterDelegate, const FPlayerInfo&, PlayerData);



UCLASS()
class MULTIVIEWER0401_API AUDPNetwork : public AActor
{
	GENERATED_BODY()

public:
	AUDPNetwork();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:
	bool IsUDP;

	UFUNCTION(BlueprintCallable, Category = "UDP")
		bool RamaUDPSender_SendString(FString ToSend); //�����ַ���

	UFUNCTION(BlueprintCallable, Category = "UDP")
		bool SendPackage(const FNetworkPackage& package); //���Ͷ���õ����ݰ��ṹ��

public:
	TSharedPtr<FInternetAddr> RemoteAddr;

	FSocket* SenderSocket;

	FUdpSocketReceiver* Receiver;

	UFUNCTION(BlueprintCallable, Category = "UDP")
		bool StartUDPSender(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort, bool UDP);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UDP")
		bool ShowOnScreenDebugMessages;

	void OnUdpDataReady(const FArrayReaderPtr& ArrayReader, const FIPv4Endpoint& Endpoint);

	UPROPERTY(BlueprintAssignable, Category = "UDP")
		FRegisterResponseDelegate OnRegisterResponse;

	UPROPERTY(BlueprintAssignable, Category = "UDP")
		FServerFrameDelegate OnServerFrame;

	UPROPERTY(BlueprintAssignable, Category = "UDP")
		FUDPMessageDelegate OnServerMessage;

	UPROPERTY(BlueprintAssignable, Category = "UDP")
		FPositionSyncDelegate OnPositionSync;

	UPROPERTY(BlueprintAssignable, Category = "UDP")
		FPlayerEnterDelegate OnPlayerEnter;

	UFUNCTION(BlueprintCallable, Category = "InterSight")
		static void GenerateRegisterPackage(const FString& name, FNetworkPackage& package);

	UFUNCTION(BlueprintCallable, Category = "InterSight")
		static void GenerateClientFramePackage(const FClientFrame& frame, FNetworkPackage& package);

	UFUNCTION(BlueprintCallable, Category = "InterSight")
		static void GenerateSyncPackage(const FClientPositionSync& syncData, FNetworkPackage& package);

	UFUNCTION(BlueprintCallable, Category = "InterSight")
		bool SendFrame(const FClientFrame& frame);

	//ScreenMsg
	FORCEINLINE void ScreenMsg(const FString& Msg)
	{
		if (!ShowOnScreenDebugMessages)
			return;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *Msg);
	}
	FORCEINLINE void ScreenMsg(const FString& Msg, const float Value)
	{
		if (!ShowOnScreenDebugMessages)
			return;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s %f"), *Msg, Value));
	}
	FORCEINLINE void ScreenMsg(const FString& Msg, const FString& Msg2)
	{
		if (!ShowOnScreenDebugMessages)
			return;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s %s"), *Msg, *Msg2));
	}

public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


private:
	static ClientFrame* cacheFrame;
	static CameraAxisInput* cacheInput;
	static ClientPositionSync* cacheSync;
	static PositionData* cachePosition;
	RouteMsg* cacheMsg;

	static google::protobuf::Arena arena;

	static void InputToMsg(FThirdPlayerInput thirdInput);
	static void FrameToMsg(FClientFrame clientFrame);

};
