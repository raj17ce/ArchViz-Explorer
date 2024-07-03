// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizModes/ArchVizMode.h"
#include "ArchVizActors/RoadActor.h"
#include "ArchVizActors/BuildingActors/WallActor.h"
#include "ArchVizActors/BuildingActors/FloorActor.h"
#include "ArchVizActors/BuildingActors/RoofActor.h"
#include "ArchVizActors/BuildingActors/DoorActor.h"
#include "ArchVizActors/InteriorActor.h"
#include "SaveGameMode.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORERTOOL_API USaveGameMode : public UArchVizMode {
	GENERATED_BODY()

public:
	USaveGameMode();

	virtual void Setup() override;
	virtual void Cleanup() override;
	virtual void EnterMode() override;
	virtual void ExitMode() override;
	virtual void SetupInputComponent() override;

protected:
	UPROPERTY()
	TArray<FString> SlotsList;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARoadActor> RoadActorClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWallActor> WallActorClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFloorActor> FloorActorClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARoofActor> RoofActorClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADoorActor> DoorActorClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AInteriorActor> InteriorActorClass;

private:
	FString CurrentSlotName;

	UFUNCTION()
	void HandleSaveButtonClick();

	UFUNCTION()
	void HandleNewProjectButtonClick();

	UFUNCTION()
	void HandleSaveProjectButtonClick();

	UFUNCTION()
	void HandleSlotItemNameButtonClick(const FString& SlotName);
	
	UFUNCTION()
	void HandleSlotItemDeleteButtonClick(const FString& SlotName);

	void DeleteSlotData(const FString& SlotName);

	TArray<FString> GetSavedSlotsList();

	void SaveGame(const FString& SlotName);
	void LoadGame(const FString& SlotName);

	void PopulateSlotsList();
	void ClearWorld();
};
