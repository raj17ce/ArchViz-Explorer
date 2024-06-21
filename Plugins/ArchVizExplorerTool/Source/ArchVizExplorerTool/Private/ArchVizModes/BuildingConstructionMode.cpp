// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingConstructionMode.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/StaticMeshComponent.h"

void UBuildingConstructionMode::Setup() {
	if (WallSubModeClass) {
		WallSubMode = NewObject<UWallSubMode>(this, WallSubModeClass);
		WallSubMode->Setup();
	}
	if (DoorSubModeClass) {
		DoorSubMode = NewObject<UDoorSubMode>(this, DoorSubModeClass);
		DoorSubMode->Setup();
	}
	if (FloorSubModeClass) {
		FloorSubMode = NewObject<UFloorSubMode>(this, FloorSubModeClass);
		FloorSubMode->Setup();
	}
	if (RoofSubModeClass) {
		RoofSubMode = NewObject<URoofSubMode>(this, RoofSubModeClass);
		RoofSubMode->Setup();
	}

	if (IsValid(WallSubMode)) {
		CurrentBuildingSubMode = EBuildingSubMode::WallConstruction;
		CurrentBuildingSubModePtr = WallSubMode;
	}
}

void UBuildingConstructionMode::EnterMode() {
	if (CurrentBuildingSubModePtr) {
		CurrentBuildingSubModePtr->EnterSubMode();
	}
}

void UBuildingConstructionMode::ExitMode() {
	if (CurrentBuildingSubModePtr) {
		CurrentBuildingSubModePtr->ExitSubMode();
	}
}

void UBuildingConstructionMode::SetPlayerController(APlayerController* Controller) {
	if (IsValid(WallSubMode)) {
		WallSubMode->SetPlayerController(Controller);
	}
 	if (IsValid(DoorSubMode)) {
		DoorSubMode->SetPlayerController(Controller);
	}
	if (IsValid(FloorSubMode)) {
		FloorSubMode->SetPlayerController(Controller);
	}
	if (IsValid(RoofSubMode)) {
		RoofSubMode->SetPlayerController(Controller);
	}
}

void UBuildingConstructionMode::SetupInputComponent() {
	if (IsValid(WallSubMode)) {
		WallSubMode->SetupInputComponent();
	}
	if (IsValid(DoorSubMode)) {
		DoorSubMode->SetupInputComponent();
	}
	if (IsValid(FloorSubMode)) {
		FloorSubMode->SetupInputComponent();
	}
	if (IsValid(RoofSubMode)) {
		RoofSubMode->SetupInputComponent();
	}
}

void UBuildingConstructionMode::HandleBuildingSubModeChange(EBuildingSubMode NewBuildingSubMode) {
	CurrentBuildingSubMode = NewBuildingSubMode;

	UpdateBuildingSubMode();
}

void UBuildingConstructionMode::UpdateBuildingSubMode() {
	switch (CurrentBuildingSubMode) {
	case EBuildingSubMode::WallConstruction:
		SetBuildingSubMode(WallSubMode);
		break;
	case EBuildingSubMode::DoorConstruction:
		SetBuildingSubMode(DoorSubMode);
		break;
	case EBuildingSubMode::FloorConstruction:
		SetBuildingSubMode(FloorSubMode);
		break;
	case EBuildingSubMode::RoofConstruction:
		SetBuildingSubMode(RoofSubMode);
		break;
	}
}

void UBuildingConstructionMode::SetBuildingSubMode(IBuildingConstructionSubMode* NewBuildingSubModePtr) {
	if (CurrentBuildingSubModePtr) {
		CurrentBuildingSubModePtr->ExitSubMode();
	}

	CurrentBuildingSubModePtr = NewBuildingSubModePtr;

	if (CurrentBuildingSubModePtr) {
		CurrentBuildingSubModePtr->EnterSubMode();
	}
}
