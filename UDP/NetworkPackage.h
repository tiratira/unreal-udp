// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "iostream"
#include "InsPackageType.h"
#include "../ServerFrame.h"
#include "NetworkPackage.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct MULTIVIEWER0401_API FNetworkPackage
{
	GENERATED_BODY()

public:

	FNetworkPackage() {};
	EPackageType type;

	std::string byteString;
};
