// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuildingConstructionSubmode.h"
#include "InputMappingContext.h"
#include "ArchVizActors/BuildingActors/FloorActor.h"
#include "FloorSubMode.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORERTOOL_API UFloorSubMode : public UBuildingConstructionSubMode {
	GENERATED_BODY()

public:
	virtual void Setup() override;
	virtual void Cleanup() override;
	virtual void EnterSubMode() override;
	virtual void ExitSubMode() override;
	virtual void SetupInputComponent() override;
	virtual void BindPropertyDelegatesToActor(ABuildingActor* Actor) override;

	void SelectActor(AFloorActor* FloorActor);
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFloorActor> FloorActorClass;

private:
	void HandleLeftMouseClick();
	void HandleRKeyPress();
	void HandleMKeyPress();

	void HandleFreeState();
	void HandleOldObjectState();
	void HandleNewObjectState();

	UFUNCTION()
	void HandleFloorSpinBoxValueChange(float InLength);

	UFUNCTION()
	void HandleFloorNewButtonClick();

	UFUNCTION()
	void HandleFloorDeleteButtonClick();

	UFUNCTION()
	void HandleFloorCloseButtonClick();

	UPROPERTY()
	AFloorActor* CurrentFloorActor;

	bool bNewFloorStart;
};
