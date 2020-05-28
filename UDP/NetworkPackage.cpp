// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkPackage.h"
#include "../Proto/command.pb.h"
#include "iostream"

//void UNetworkPackage::GenerateInputPackage(const FThirdPlayerInput& input, UNetworkPackage*& package)
//{
//	package = NewObject<UNetworkPackage>();
//	package->type = EPackageType::T_FRAME;
//	ThirdInput inputMsg = ThirdInput();
//	inputMsg.set_x(input.axis.X);
//	inputMsg.set_y(input.axis.Y);
//	inputMsg.set_z(input.axis.Z);
//	inputMsg.set_jump(input.jump);
//	std::string msgString;
//	inputMsg.SerializeToString(&msgString);
//	package->byteString = msgString;
//}

//void UNetworkPackage::GenerateRegisterPackage(const FString& name, FNetworkPackage& package)
//{
//	std::string str(TCHAR_TO_UTF8(*name));
//	RegisterMsg registerMsg = RegisterMsg();
//	registerMsg.set_title(str);
//	std::string out;
//	registerMsg.SerializeToString(&out);
//
//	type = EPackageType::T_REGISTER;
//	byteString = out;
//
//	package = this;
//}
//
//void UNetworkPackage::GenerateClientFramePackage(const FClientFrame& frame, FNetworkPackage& package)
//{
//	ClientFrame frameCache;
//	frameCache.set_clientid(frame.clientId);
//	FThirdPlayerInput::ToProto(frame.input, &frameCache);
//	frameCache.set_allocated_input(&frameCache);
//	std::string frame_out_buffer;
//	frameCache.SerializeToString(&frame_out_buffer);
//
//	//package组织
//	type = EPackageType::T_CLIENT_FRAME;
//	byteString = frame_out_buffer;
//
//	package = this;
//}
