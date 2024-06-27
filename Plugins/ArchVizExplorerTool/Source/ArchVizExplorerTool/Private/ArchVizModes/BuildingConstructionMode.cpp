// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingConstructionMode.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Widgets/BuildingConstructionWidget.h"

void UBuildingConstructionMode::Setup() {
	
	if (IsValid(WidgetClass)) {
		Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass, "Building Widget");
		auto* BuildingWidget = Cast<UBuildingConstructionWidget>(Widget);
		if (BuildingWidget) {
			BuildingWidget->OnBuildingSubModeChange.AddUObject(this, &UBuildingConstructionMode::HandleBuildingSubModeChange);
		}
	}

	if (WallSubModeClass) {
		WallSubMode = NewObject<UWallSubMode>(this, WallSubModeClass);
		WallSubMode->Setup();
		WallSubMode->OnOtherBuildingActorSelected.BindUObject(this, &UBuildingConstructionMode::HandleBuildingActorSelected);
	}
	if (DoorSubModeClass) {
		DoorSubMode = NewObject<UDoorSubMode>(this, DoorSubModeClass);
		DoorSubMode->Setup();
		DoorSubMode->OnOtherBuildingActorSelected.BindUObject(this, &UBuildingConstructionMode::HandleBuildingActorSelected);
	}
	if (FloorSubModeClass) {
		FloorSubMode = NewObject<UFloorSubMode>(this, FloorSubModeClass);
		FloorSubMode->Setup();
		FloorSubMode->OnOtherBuildingActorSelected.BindUObject(this, &UBuildingConstructionMode::HandleBuildingActorSelected);
	}
	if (RoofSubModeClass) {
		RoofSubMode = NewObject<URoofSubMode>(this, RoofSubModeClass);
		RoofSubMode->Setup();
		RoofSubMode->OnOtherBuildingActorSelected.BindUObject(this, &UBuildingConstructionMode::HandleBuildingActorSelected);
	}

	if (IsValid(WallSubMode)) {
		CurrentBuildingSubMode = EBuildingSubMode::WallConstruction;
		CurrentBuildingSubModePtr = WallSubMode;
	}
}

void UBuildingConstructionMode::EnterMode() {
	ShowWidget();
	if (CurrentBuildingSubModePtr) {
		CurrentBuildingSubModePtr->EnterSubMode();
	}
}

void UBuildingConstructionMode::ExitMode() {
	HideWidget();
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
	if (auto* BuildingWidget = Cast<UBuildingConstructionWidget>(Widget)) {
		BuildingWidget->HighlightSelectedColour(NewBuildingSubMode);
	}
}

void UBuildingConstructionMode::HandleBuildingActorSelected(EBuildingSubMode NewBuildingSubMode, AActor* Actor) {
	HandleBuildingSubModeChange(NewBuildingSubMode);
	UpdateSelectedActor(Actor);
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

void UBuildingConstructionMode::SetBuildingSubMode(UBuildingConstructionSubMode* NewBuildingSubModePtr) {
	if (CurrentBuildingSubModePtr) {
		CurrentBuildingSubModePtr->ExitSubMode();
	}

	CurrentBuildingSubModePtr = NewBuildingSubModePtr;

	if (CurrentBuildingSubModePtr) {
		CurrentBuildingSubModePtr->EnterSubMode();
	}
}

void UBuildingConstructionMode::UpdateSelectedActor(AActor* Actor) {
	switch (CurrentBuildingSubMode) {
	case EBuildingSubMode::WallConstruction:
		if (auto* WallActor = Cast<AWallActor>(Actor)) {
			if (IsValid(WallSubMode)) {
				WallSubMode->SelectActor(WallActor);
			}
		}
		break;
	case EBuildingSubMode::DoorConstruction:
		if (auto* DoorActor = Cast<ADoorActor>(Actor)) {
			if (IsValid(DoorSubMode)) {
				DoorSubMode->SelectActor(DoorActor);
			}
		}
		break;
	case EBuildingSubMode::FloorConstruction:
		if (auto* FloorActor = Cast<AFloorActor>(Actor)) {
			if (IsValid(FloorSubMode)) {
				FloorSubMode->SelectActor(FloorActor);
			}
		}
		break;
	case EBuildingSubMode::RoofConstruction:
		if (auto* RoofActor = Cast<ARoofActor>(Actor)) {
			if (IsValid(RoofSubMode)) {
				RoofSubMode->SelectActor(RoofActor);
			}
		}
		break;
	}
}
