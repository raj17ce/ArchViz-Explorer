// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuildingConstructionSubmode.h"
#include "InputMappingContext.h"
#include "DoorSubMode.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORERTOOL_API UDoorSubMode : public UObject, public IBuildingConstructionSubMode {
	GENERATED_BODY()

public:
	virtual void Setup() override;
	virtual void EnterSubMode() override;
	virtual void ExitSubMode() override;
	virtual void SetPlayerController(APlayerController* Controller) override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	UInputMappingContext* MappingContext;

	void HandleLeftMouseClick();
};
