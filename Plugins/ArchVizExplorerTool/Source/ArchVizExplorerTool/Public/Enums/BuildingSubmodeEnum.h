// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSubmodeEnum.generated.h"

UENUM(BlueprintType)
enum class EBuildingSubmode : uint8 {
	WallGeneration,
	DoorGeneration,
	FloorGeneration,
	RoofGeneration
};
