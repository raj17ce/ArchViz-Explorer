// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizModeEnum.generated.h"

UENUM(BlueprintType)
enum class EArchVizMode : uint8 {
	RoadConstruction,
	BuildingConstruction,
	InteriorDesign,
	SaveGame
};
