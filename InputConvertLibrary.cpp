// Fill out your copyright notice in the Description page of Project Settings.


#include "InputConvertLibrary.h"
#include "iostream"
#include "./Proto/command.pb.h"

//FString UInputConvertLibrary::ThirdInputToString(const FThirdPlayerInput& input) {
//	ThirdInput msg = ThirdInput();
//	msg.set_x(input.axis.X);
//	msg.set_y(input.axis.Y);
//	msg.set_z(input.axis.Z);
//	msg.set_jump(input.jump);
//	std::string str;
//	msg.SerializeToString(&str);
//	FString out(str.c_str());
//	return out;
//}

//FThirdPlayerInput UInputConvertLibrary::StringToInput(const FString& message) {
//	std::string str(TCHAR_TO_UTF8(*message));
//	ThirdInput input;
//	input.ParseFromString(str);
//	FThirdPlayerInput out = FThirdPlayerInput(input.x(), input.y(), input.z(), input.jump());
//	return out;
//}

UInputConvertLibrary::UInputConvertLibrary() {
	//缺省构造函数
}