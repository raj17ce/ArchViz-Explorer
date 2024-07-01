// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteriorAssetTypeEnum.generated.h"

UENUM(BlueprintType)
enum class EInteriorAssetType : uint8 {
	FloorPlaceable,
	WallPlaceable,
	RoofPlaceable,
	InteriorPlaceable
};