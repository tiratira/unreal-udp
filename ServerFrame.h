// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Proto/command.pb.h"
#include "ServerFrame.generated.h"

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "InterSight")
		int Id;

	UPROPERTY(BlueprintReadWrite, Category = "InterSight")
		FString Title;
};

USTRUCT(BlueprintType)
struct MULTIVIEWER0401_API FThirdPlayerInput
{

	GENERATED_BODY()

public:
	FThirdPlayerInput()
	{
		cameraYaw = 0;
		moveRight = 0;
		moveForward = 0;
		jump = false;
	};

	FThirdPlayerInput(float y, float r, float f, bool j)
	{
		cameraYaw = y;
		moveRight = r;
		moveForward = f;
		jump = j;
	};

	FThirdPlayerInput(CameraAxisInput* input);

	UPROPERTY(BlueprintReadWrite, Category = "InterSight")
		bool jump;

	UPROPERTY(BlueprintReadWrite, Category = "InterSight")
		float cameraYaw;

	UPROPERTY(BlueprintReadWrite, Category = "InterSight")
		float moveRight;

	UPROPERTY(BlueprintReadWrite, Category = "InterSight")
		float moveForward;

};

USTRUCT(BlueprintType)
struct MULTIVIEWER0401_API FClientFrame
{
	GENERATED_BODY()
public:
	FClientFrame() {};
	FClientFrame(ClientFrame* clientFrame);

	UPROPERTY(BlueprintReadWrite, Category = "InterSight")
		int32 clientId;
	UPROPERTY(BlueprintReadWrite, Category = "InterSight")
		FThirdPlayerInput input;

};

/**
 *
 */
USTRUCT(BlueprintType)
struct MULTIVIEWER0401_API FServerFrame
{
	GENERATED_BODY()
public:
	FServerFrame() {};
	FServerFrame(ServerFrame* serverFrame);

	UPROPERTY(BlueprintReadOnly, Category = "InterSight")
		int32 frameId;

	UPROPERTY(BlueprintReadOnly, Category = "InterSight")
		TArray<FClientFrame> clientFrames;

	//ServerFrame ToProto();
};

USTRUCT(BlueprintType)
struct MULTIVIEWER0401_API FServerFramePackage
{
	GENERATED_BODY()
public:
	FServerFramePackage() {};
	FServerFramePackage(ServerFramePackage* package);

	UPROPERTY(BlueprintReadOnly, Category = "InterSight")
		FServerFrame current;
	UPROPERTY(BlueprintReadOnly, Category = "InterSight")
		FServerFrame cache1;
	UPROPERTY(BlueprintReadOnly, Category = "InterSight")
		FServerFrame cache2;

	//ServerFramePackage ToProto();
};

USTRUCT(BlueprintType)
struct MULTIVIEWER0401_API FServerResigterResponse
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "InterSight")
		bool result;

	UPROPERTY(BlueprintReadOnly, Category = "InterSight")
		int id;

	UPROPERTY(BlueprintReadOnly, Category = "InterSight")
		int port;

	UPROPERTY(BlueprintReadOnly, Category = "InterSight")
		int frameId;

	UPROPERTY(BlueprintReadOnly, Category = "InterSight")
		TArray<FPlayerInfo> players;
};

USTRUCT(BlueprintType)
struct MULTIVIEWER0401_API FClientPositionSync
{
	GENERATED_BODY()
public:
	FClientPositionSync() {};

	UPROPERTY(BlueprintReadWrite, Category = "InterSight")
		int clientId;

	UPROPERTY(BlueprintReadWrite, Category = "InterSight")
		FVector position;
};
