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

void UArchVizModeWidget::HandleSaveGameModeButtonClick() {
	OnArchVizModeChange.Broadcast(EArchVizMode::SaveGame);
}

void UArchVizModeWidget::NativeOnInitialized() {
	Super::NativeOnInitialized();

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

	if (IsValid(SaveGameModeButton)) {
		SaveGameModeButton->OnClicked.AddDynamic(this, &UArchVizModeWidget::HandleSaveGameModeButtonClick);
	}
}

void UArchVizModeWidget::HighlightSelectedColour(EArchVizMode NewArchVizMode) {
	RemoveHighlights();
	switch (NewArchVizMode) {
	case EArchVizMode::RoadConstruction:
		if (IsValid(RoadModeButton)) {
			FButtonStyle ButtonStyle = RoadModeButton->GetStyle();
			SetHighlightedTint(ButtonStyle);
			RoadModeButton->SetStyle(ButtonStyle);
		}
		break;
	case EArchVizMode::BuildingConstruction:
		if (IsValid(BuildingModeButton)) {
			FButtonStyle ButtonStyle = BuildingModeButton->GetStyle();
			SetHighlightedTint(ButtonStyle);
			BuildingModeButton->SetStyle(ButtonStyle);
		}
		break;
	case EArchVizMode::InteriorDesign:
		if (IsValid(InteriorModeButton)) {
			FButtonStyle ButtonStyle = InteriorModeButton->GetStyle();
			SetHighlightedTint(ButtonStyle);
			InteriorModeButton->SetStyle(ButtonStyle);
		}
		break;
	case EArchVizMode::SaveGame:
		if (IsValid(SaveGameModeButton)) {
			FButtonStyle ButtonStyle = SaveGameModeButton->GetStyle();
			SetHighlightedTint(ButtonStyle);
			SaveGameModeButton->SetStyle(ButtonStyle);
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

void UArchVizModeWidget::RemoveHighlights() {
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
		SetNormalTint(ButtonStyle);
		InteriorModeButton->SetStyle(ButtonStyle);
	}
	if (IsValid(SaveGameModeButton)) {
		FButtonStyle ButtonStyle = SaveGameModeButton->GetStyle();
		SetNormalTint(ButtonStyle);
		SaveGameModeButton->SetStyle(ButtonStyle);
	}
}
