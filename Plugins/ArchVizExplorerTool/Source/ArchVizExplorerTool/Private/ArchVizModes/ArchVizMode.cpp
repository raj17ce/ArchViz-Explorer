// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/ArchVizMode.h"

void UArchVizMode::SetPlayerController(APlayerController* Controller) {
	PlayerController = Controller;
}

void UArchVizMode::ShowWidget() {
	if (IsValid(Widget)) {
		Widget->AddToViewport();
	}
}

void UArchVizMode::HideWidget() {
	if (IsValid(Widget)) {
		Widget->RemoveFromParent();
	}
}
