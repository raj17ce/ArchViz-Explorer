// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Enums/BuildingSubModeEnum.h"
#include "ArchVizActors/BuildingActor.h"
#include "BuildingConstructionSubMode.generated.h"

UENUM(BlueprintType)
enum class EBuildingSubModeState : uint8 {
	Free,
	OldObject,
	NewObject
};

class UInputMappingContext;
class APlayerController;

DECLARE_DELEGATE_TwoParams(FOnOtherBuildingActorSelected, EBuildingSubMode, AActor*)

/**
 * 
 */
UCLASS(ABSTRACT)
class ARCHVIZEXPLORERTOOL_API UBuildingConstructionSubMode : public UObject {
	GENERATED_BODY()

public:
	//Pure-Virtual
	virtual void Setup() PURE_VIRTUAL(UArchVizMode::Setup, );
	virtual void Cleanup() PURE_VIRTUAL(UArchVizMode::Cleanup, );
	virtual void EnterSubMode() PURE_VIRTUAL(UArchVizMode::EnterSubMode, );
	virtual void ExitSubMode() PURE_VIRTUAL(UArchVizMode::ExitSubMode, );
	virtual void SetupInputComponent() PURE_VIRTUAL(UArchVizMode::SetupInputComponent, );

	//Virtual
	virtual void SetPlayerController(APlayerController* Controller);
	virtual void BindPropertyDelegatesToActor(ABuildingActor* Actor);

	FHitResult GetHitResult(const TArray<AActor*>& ActorsToIgnore = TArray<AActor*>{}) const;

	FOnOtherBuildingActorSelected OnOtherBuildingActorSelected;

protected:
	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	UInputMappingContext* MappingContext;

	EBuildingSubModeState SubModeState;
};
