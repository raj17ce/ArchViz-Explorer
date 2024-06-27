// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/BuildingConstructionWidget.h"

void UBuildingConstructionWidget::HandleWallSubModeButtonClick() {
	OnBuildingSubModeChange.Broadcast(EBuildingSubMode::WallConstruction);
}

void UBuildingConstructionWidget::HandleDoorSubModeButtonClick() {
	OnBuildingSubModeChange.Broadcast(EBuildingSubMode::DoorConstruction);
}

void UBuildingConstructionWidget::HandleFloorSubModeButtonClick() {
	OnBuildingSubModeChange.Broadcast(EBuildingSubMode::FloorConstruction);
}

void UBuildingConstructionWidget::HandleRoofSubModeButtonClick() {
	OnBuildingSubModeChange.Broadcast(EBuildingSubMode::RoofConstruction);
}

void UBuildingConstructionWidget::NativeConstruct() {
	Super::NativeConstruct();

	if (IsValid(WallSubModeButton)) {
		WallSubModeButton->OnClicked.AddDynamic(this, &UBuildingConstructionWidget::HandleWallSubModeButtonClick);
	}

	if (IsValid(DoorSubModeButton)) {
		DoorSubModeButton->OnClicked.AddDynamic(this, &UBuildingConstructionWidget::HandleDoorSubModeButtonClick);
	}

	if (IsValid(FloorSubModeButton)) {
		FloorSubModeButton->OnClicked.AddDynamic(this, &UBuildingConstructionWidget::HandleFloorSubModeButtonClick);
	}	

	if (IsValid(RoofSubModeButton)) {
		RoofSubModeButton->OnClicked.AddDynamic(this, &UBuildingConstructionWidget::HandleRoofSubModeButtonClick);
	}
}

void UBuildingConstructionWidget::HighlightSelectedColour(EBuildingSubMode NewBuildingSubMode) {
	switch (NewBuildingSubMode) {
	case EBuildingSubMode::WallConstruction:
		if (IsValid(WallSubModeButton)) {
			FButtonStyle ButtonStyle = WallSubModeButton->GetStyle();
			SetHighlightedTint(ButtonStyle);
			WallSubModeButton->SetStyle(ButtonStyle);
		}
		if (IsValid(DoorSubModeButton)) {
			FButtonStyle ButtonStyle = DoorSubModeButton->GetStyle();
			SetNormalTint(ButtonStyle);
			DoorSubModeButton->SetStyle(ButtonStyle);
		}
		if (IsValid(FloorSubModeButton)) {
			FButtonStyle ButtonStyle = FloorSubModeButton->GetStyle();
			SetNormalTint(ButtonStyle);
			FloorSubModeButton->SetStyle(ButtonStyle);
		}
		if (IsValid(RoofSubModeButton)) {
			FButtonStyle ButtonStyle = RoofSubModeButton->GetStyle();
			SetNormalTint(ButtonStyle);
			RoofSubModeButton->SetStyle(ButtonStyle);
		}
		break;
	case EBuildingSubMode::DoorConstruction:
		if (IsValid(WallSubModeButton)) {
			FButtonStyle ButtonStyle = WallSubModeButton->GetStyle();
			SetNormalTint(ButtonStyle);
			WallSubModeButton->SetStyle(ButtonStyle);
		}
		if (IsValid(DoorSubModeButton)) {
			FButtonStyle ButtonStyle = DoorSubModeButton->GetStyle();
			SetHighlightedTint(ButtonStyle);
			DoorSubModeButton->SetStyle(ButtonStyle);
		}
		if (IsValid(FloorSubModeButton)) {
			FButtonStyle ButtonStyle = FloorSubModeButton->GetStyle();
			SetNormalTint(ButtonStyle);
			FloorSubModeButton->SetStyle(ButtonStyle);
		}
		if (IsValid(RoofSubModeButton)) {
			FButtonStyle ButtonStyle = RoofSubModeButton->GetStyle();
			SetNormalTint(ButtonStyle);
			RoofSubModeButton->SetStyle(ButtonStyle);
		}
		break;
	case EBuildingSubMode::FloorConstruction:
		if (IsValid(WallSubModeButton)) {
			FButtonStyle ButtonStyle = WallSubModeButton->GetStyle();
			SetNormalTint(ButtonStyle);
			WallSubModeButton->SetStyle(ButtonStyle);
		}
		if (IsValid(DoorSubModeButton)) {
			FButtonStyle ButtonStyle = DoorSubModeButton->GetStyle();
			SetNormalTint(ButtonStyle);
			DoorSubModeButton->SetStyle(ButtonStyle);
		}
		if (IsValid(FloorSubModeButton)) {
			FButtonStyle ButtonStyle = FloorSubModeButton->GetStyle();
			SetHighlightedTint(ButtonStyle);
			FloorSubModeButton->SetStyle(ButtonStyle);
		}
		if (IsValid(RoofSubModeButton)) {
			FButtonStyle ButtonStyle = RoofSubModeButton->GetStyle();
			SetNormalTint(ButtonStyle);
			RoofSubModeButton->SetStyle(ButtonStyle);
		}
		break;
	case EBuildingSubMode::RoofConstruction:
		if (IsValid(WallSubModeButton)) {
			FButtonStyle ButtonStyle = WallSubModeButton->GetStyle();
			SetNormalTint(ButtonStyle);
			WallSubModeButton->SetStyle(ButtonStyle);
		}
		if (IsValid(DoorSubModeButton)) {
			FButtonStyle ButtonStyle = DoorSubModeButton->GetStyle();
			SetNormalTint(ButtonStyle);
			DoorSubModeButton->SetStyle(ButtonStyle);
		}
		if (IsValid(FloorSubModeButton)) {
			FButtonStyle ButtonStyle = FloorSubModeButton->GetStyle();
			SetNormalTint(ButtonStyle);
			FloorSubModeButton->SetStyle(ButtonStyle);
		}
		if (IsValid(RoofSubModeButton)) {
			FButtonStyle ButtonStyle = RoofSubModeButton->GetStyle();
			SetHighlightedTint(ButtonStyle);
			RoofSubModeButton->SetStyle(ButtonStyle);
		}
		break;
	}
}

void UBuildingConstructionWidget::SetNormalTint(FButtonStyle& ButtonStyle) {
	FSlateColor NewNormalTint = FSlateColor(FLinearColor(0.012983f, 0.012983f, 0.012983f));
	ButtonStyle.Normal.TintColor = NewNormalTint;
}

void UBuildingConstructionWidget::SetHighlightedTint(FButtonStyle& ButtonStyle) {
	FSlateColor NewNormalTint = FSlateColor(FLinearColor(0.354167f, 0.354167f, 0.354167f));
	ButtonStyle.Normal.TintColor = NewNormalTint;
}
