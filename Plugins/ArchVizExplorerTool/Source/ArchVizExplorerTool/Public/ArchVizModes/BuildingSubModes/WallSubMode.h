// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuildingConstructionSubmode.h"
#include "InputMappingContext.h"
#include "ArchVizActors/BuildingActors/WallActor.h"
#include "WallSubMode.generated.h"

class APlayerController;

/**
 *
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORERTOOL_API UWallSubMode : public UBuildingConstructionSubMode {
	GENERATED_BODY()

public:
	virtual void Setup() override;
	virtual void Cleanup() override;
	virtual void EnterSubMode() override;
	virtual void ExitSubMode() override;
	virtual void SetupInputComponent() override;

	void SelectActor(AWallActor* WallActor);
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWallActor> WallActorClass;

private:
	void HandleLeftMouseClick();
	void HandleRKeyPress();
	void HandleMKeyPress();

	void HandleFreeState();
	void HandleOldObjectState();
	void HandleNewObjectState();

	void BindWidgetDelegates();

	UFUNCTION()
	void HandleWallLengthSpinBoxValueChange(float InLength);

	UFUNCTION()
	void HandleWallNewButtonClick();

	UFUNCTION()
	void HandleWallDeleteButtonClick();

	UFUNCTION()
	void HandleWallCloseButtonClick();

	UPROPERTY()
	AWallActor* CurrentWallActor;

	bool bNewWallStart;
};
