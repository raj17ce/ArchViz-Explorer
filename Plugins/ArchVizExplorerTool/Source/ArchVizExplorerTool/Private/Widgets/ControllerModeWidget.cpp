// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ControllerModeWidget.h"

void UControllerModeWidget::HandleRoadModeButtonClick() {
	OnControllerModeChange.Broadcast(EControllerMode::RoadConstruction);
}

void UControllerModeWidget::HandleBuildingModeButtonClick() {
	OnControllerModeChange.Broadcast(EControllerMode::BuildingConstruction);
}

void UControllerModeWidget::HandleInteriorModeButtonClick() {
	OnControllerModeChange.Broadcast(EControllerMode::InteriorDesign);
}

void UControllerModeWidget::NativeConstruct() {
	Super::NativeConstruct();

	OnControllerModeChange.AddUObject(this, &UControllerModeWidget::HighlightSelectedColour);

	if (IsValid(RoadModeButton)) {
		RoadModeButton->OnClicked.AddDynamic(this, &UControllerModeWidget::HandleRoadModeButtonClick);
	}

	if (IsValid(BuildingModeButton)) {
		BuildingModeButton->OnClicked.AddDynamic(this, &UControllerModeWidget::HandleBuildingModeButtonClick);
	}

	if (IsValid(InteriorModeButton)) {
		InteriorModeButton->OnClicked.AddDynamic(this, &UControllerModeWidget::HandleInteriorModeButtonClick);
	}
}

void UControllerModeWidget::HighlightSelectedColour(EControllerMode NewControllerMode) {
	switch (NewControllerMode) {
	case EControllerMode::RoadConstruction:
		if (IsValid(RoadModeButton)) {
			FButtonStyle ButtonStyle = RoadModeButton->GetStyle();
			SetHighlightedTint(ButtonStyle);
			RoadModeButton->SetStyle(ButtonStyle);
		}
		if (IsValid(BuildingModeButton)) {
			FButtonStyle ButtonStyle = BuildingModeButton->GetStyle();
			SetNormalTint(ButtonStyle);
			BuildingModeButton->SetStyle(ButtonStyle);
		}
		if (IsValid(InteriorModeButton)) {
			FButtonStyle ButtonStyle = InteriorModeButton->GetStyle();
			SetNormalTint(ButtonStyle);
			InteriorModeButton->SetStyle(ButtonStyle);
		}
		break;
	case EControllerMode::BuildingConstruction:
		if (IsValid(RoadModeButton)) {
			FButtonStyle ButtonStyle = RoadModeButton->GetStyle();
			SetNormalTint(ButtonStyle);
			RoadModeButton->SetStyle(ButtonStyle);
		}
		if (IsValid(BuildingModeButton)) {
			FButtonStyle ButtonStyle = BuildingModeButton->GetStyle();
			SetHighlightedTint(ButtonStyle);
			BuildingModeButton->SetStyle(ButtonStyle);
		}
		if (IsValid(InteriorModeButton)) {
			FButtonStyle ButtonStyle = InteriorModeButton->GetStyle();
			SetNormalTint(ButtonStyle);
			InteriorModeButton->SetStyle(ButtonStyle);
		}
		break;
	case EControllerMode::InteriorDesign:
		if (IsValid(RoadModeButton)) {
			FButtonStyle ButtonStyle = RoadModeButton->GetStyle();
			SetNormalTint(ButtonStyle);
			RoadModeButton->SetStyle(ButtonStyle);
		}
		if (IsValid(BuildingModeButton)) {
			FButtonStyle ButtonStyle = BuildingModeButton->GetStyle();
			SetNormalTint(ButtonStyle);
			BuildingModeButton->SetStyle(ButtonStyle);
		}
		if (IsValid(InteriorModeButton)) {
			FButtonStyle ButtonStyle = InteriorModeButton->GetStyle();
			SetHighlightedTint(ButtonStyle);
			InteriorModeButton->SetStyle(ButtonStyle);
		}
		break;
	}
}

void UControllerModeWidget::SetNormalTint(FButtonStyle& ButtonStyle) {
	FSlateColor NewNormalTint = FSlateColor(FLinearColor(0.012983f, 0.012983f, 0.012983f));
	ButtonStyle.Normal.TintColor = NewNormalTint;
}

void UControllerModeWidget::SetHighlightedTint(FButtonStyle& ButtonStyle) {
	FSlateColor NewNormalTint = FSlateColor(FLinearColor(0.198028f, 0.198028f, 0.223958f));
	ButtonStyle.Normal.TintColor = NewNormalTint;
}
