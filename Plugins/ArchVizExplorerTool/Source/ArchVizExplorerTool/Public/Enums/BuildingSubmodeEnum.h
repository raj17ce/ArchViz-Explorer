// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSubModeEnum.generated.h"

UENUM(BlueprintType)
enum class EBuildingSubMode : uint8 {
	WallConstruction,
	DoorConstruction,
	FloorConstruction,
	RoofConstruction
};
