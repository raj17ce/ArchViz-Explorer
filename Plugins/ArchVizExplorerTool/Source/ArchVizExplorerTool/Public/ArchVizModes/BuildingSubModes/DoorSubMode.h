// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuildingConstructionSubmode.h"
#include "InputMappingContext.h"
#include "ArchVizActors/BuildingActors/DoorActor.h"
#include "DoorSubMode.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORERTOOL_API UDoorSubMode : public UBuildingConstructionSubMode {
	GENERATED_BODY()

public:
	virtual void Setup() override;
	virtual void Cleanup() override;
	virtual void EnterSubMode() override;
	virtual void ExitSubMode() override;
	virtual void SetupInputComponent() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADoorActor> DoorActorClass;

private:
	void HandleLeftMouseClick();
	void HandleRKeyPress();
	void HandleMKeyPress();
	void HandleOKeyPress();

	void HandleFreeState();
	void HandleNewObjectState();

	void BindWidgetDelegates();

	UFUNCTION()
	void HandleDoorNewButtonClick();

	UFUNCTION()
	void HandleDoorDeleteButtonClick();

	UFUNCTION()
	void HandleDoorCloseButtonClick();

	UPROPERTY()
	ADoorActor* CurrentDoorActor;
};
