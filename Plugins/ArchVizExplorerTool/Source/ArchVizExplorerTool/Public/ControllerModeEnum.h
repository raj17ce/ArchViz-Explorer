// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ControllerModeEnum.generated.h"

UENUM(BlueprintType)
enum class EControllerMode : uint8 {
	RoadConstruction,
	BuildingConstruction,
	InteriorDesign
};
