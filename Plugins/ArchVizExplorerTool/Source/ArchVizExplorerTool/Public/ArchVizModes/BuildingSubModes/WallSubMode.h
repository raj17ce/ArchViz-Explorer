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
class ARCHVIZEXPLORERTOOL_API UWallSubMode : public UObject, public IBuildingConstructionSubMode {
	GENERATED_BODY()

public:
	virtual void Setup() override;
	virtual void EnterSubMode() override;
	virtual void ExitSubMode() override;
	virtual void SetPlayerController(APlayerController* Controller) override;
	virtual void SetupInputComponent() override;

	AWallActor* GetWallActor() const;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWallActor> WallActorClass;

private:
    UPROPERTY()
    APlayerController* PlayerController;

    UPROPERTY()
	UInputMappingContext* MappingContext;

	UPROPERTY()
	AWallActor* WallActor;

	void HandleLeftMouseClick();
	void HandleRKeyPress();
};
