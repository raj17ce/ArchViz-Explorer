// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuildingConstructionSubmode.h"
#include "InputMappingContext.h"
#include "FloorSubMode.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORERTOOL_API UFloorSubMode : public UBuildingConstructionSubMode {
	GENERATED_BODY()

public:
	virtual void Setup() override;
	virtual void EnterSubMode() override;
	virtual void ExitSubMode() override;
	virtual void SetupInputComponent() override;

private:
	void HandleLeftMouseClick();
};
