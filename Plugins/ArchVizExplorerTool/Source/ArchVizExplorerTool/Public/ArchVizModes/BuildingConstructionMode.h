 //Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizMode.h"
#include "UObject/NoExportTypes.h"
#include "InputMappingContext.h"
#include "Enums/BuildingSubModeEnum.h"
#include "BuildingSubmodes/RoofSubmode.h"
#include "BuildingSubmodes/BuildingConstructionSubmode.h"
#include "BuildingSubmodes/WallSubmode.h"
#include "BuildingSubmodes/DoorSubmode.h"
#include "BuildingSubmodes/FloorSubmode.h"
#include "BuildingConstructionMode.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORERTOOL_API UBuildingConstructionMode : public UArchVizMode {
	GENERATED_BODY()

public:
	virtual void Setup() override;
	virtual void Cleanup() override;
	virtual void EnterMode() override;
	virtual void ExitMode() override;
	virtual void SetPlayerController(AArchVizController* Controller) override;
	virtual void SetupInputComponent() override;
	virtual void BindPropertyDelegatesToActor(AArchVizActor* Actor) override;

	UFUNCTION()
	void HandleBuildingSubModeChange(EBuildingSubMode NewBuildingSubMode);

	UFUNCTION()
	void HandleBuildingActorSelected(EBuildingSubMode NewBuildingSubMode, AActor* Actor);

private:
	EBuildingSubMode CurrentBuildingSubMode;
	UBuildingConstructionSubMode* CurrentBuildingSubModePtr;

	//SubModes
	UPROPERTY(EditDefaultsOnly, Category="SubMode")
	TSubclassOf<UWallSubMode> WallSubModeClass;

	UPROPERTY()
	UWallSubMode* WallSubMode;

	UPROPERTY(EditDefaultsOnly, Category="SubMode")
	TSubclassOf<UDoorSubMode> DoorSubModeClass;

	UPROPERTY()
	UDoorSubMode* DoorSubMode;

	UPROPERTY(EditDefaultsOnly, Category="SubMode")
	TSubclassOf<UFloorSubMode> FloorSubModeClass;

	UPROPERTY()
	UFloorSubMode* FloorSubMode;

	UPROPERTY(EditDefaultsOnly, Category="SubMode")
	TSubclassOf<URoofSubMode> RoofSubModeClass;

	UPROPERTY()
	URoofSubMode* RoofSubMode;

	//Update Sub Mode Helpers
	void UpdateBuildingSubMode();
	void SetBuildingSubMode(UBuildingConstructionSubMode* NewBuildingSubModePtr);

	void UpdateSelectedActor(AActor* Actor);
};
