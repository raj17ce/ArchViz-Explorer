// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ArchVizModeWidget.h"


void UArchVizModeWidget::HandleRoadModeButtonClick() {
	OnArchVizModeChange.Broadcast(EArchVizMode::RoadConstruction);
}

void UArchVizModeWidget::HandleBuildingModeButtonClick() {
	OnArchVizModeChange.Broadcast(EArchVizMode::BuildingConstruction);
}

void UArchVizModeWidget::HandleInteriorModeButtonClick() {
	OnArchVizModeChange.Broadcast(EArchVizMode::InteriorDesign);
}

void UArchVizModeWidget::NativeConstruct() {
	Super::NativeConstruct();

	OnArchVizModeChange.AddUObject(this, &UArchVizModeWidget::HighlightSelectedColour);

	if (IsValid(RoadModeButton)) {
		RoadModeButton->OnClicked.AddDynamic(this, &UArchVizModeWidget::HandleRoadModeButtonClick);
	}

	if (IsValid(BuildingModeButton)) {
		BuildingModeButton->OnClicked.AddDynamic(this, &UArchVizModeWidget::HandleBuildingModeButtonClick);
	}

	if (IsValid(InteriorModeButton)) {
		InteriorModeButton->OnClicked.AddDynamic(this, &UArchVizModeWidget::HandleInteriorModeButtonClick);
	}
}

void UArchVizModeWidget::HighlightSelectedColour(EArchVizMode NewArchVizMode) {
	switch (NewArchVizMode) {
	case EArchVizMode::RoadConstruction:
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
	case EArchVizMode::BuildingConstruction:
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
	case EArchVizMode::InteriorDesign:
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

void UArchVizModeWidget::SetNormalTint(FButtonStyle& ButtonStyle) {
	FSlateColor NewNormalTint = FSlateColor(FLinearColor(0.012983f, 0.012983f, 0.012983f));
	ButtonStyle.Normal.TintColor = NewNormalTint;
}

void UArchVizModeWidget::SetHighlightedTint(FButtonStyle& ButtonStyle) {
	FSlateColor NewNormalTint = FSlateColor(FLinearColor(0.354167f, 0.354167f, 0.354167f));
	ButtonStyle.Normal.TintColor = NewNormalTint;
}
