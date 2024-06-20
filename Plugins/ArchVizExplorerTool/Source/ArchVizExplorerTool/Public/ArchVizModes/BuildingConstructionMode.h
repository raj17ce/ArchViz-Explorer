// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizMode.h"
#include "UObject/NoExportTypes.h"
#include "InputMappingContext.h"
#include "WallActor.h"
#include "BuildingConstructionMode.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORERTOOL_API UBuildingConstructionMode : public UObject, public IArchVizMode {
	GENERATED_BODY()
public:
	virtual void EnterMode() override;
	virtual void ExitMode() override;
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
