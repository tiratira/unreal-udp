// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./UDP/UDPNetwork.h"
#include "UObject/NoExportTypes.h"
#include "InputConvertLibrary.generated.h"

/**
 *
 */
UCLASS()
class MULTIVIEWER0401_API UInputConvertLibrary : public UObject
{
	GENERATED_BODY()

public:
	UInputConvertLibrary();

	//UFUNCTION(BlueprintCallable, Category = "InterSight")
	//	static FString ThirdInputToString(const FThirdPlayerInput& input); //������ת������������

	//UFUNCTION(BlueprintCallable, Category = "InterSight")
	//	static FThirdPlayerInput StringToInput(const FString& message); //����������ת���ɲ���

};
