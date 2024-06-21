// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/InteriorDesignMode.h"

void UInteriorDesignMode::Setup() {
	if (IsValid(WidgetClass)) {
		Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass, "Interior Widget");
	}
}

void UInteriorDesignMode::EnterMode() {
	ShowWidget();
	//To-Do
}

void UInteriorDesignMode::ExitMode() {
	HideWidget();
	//To-Do
}

void UInteriorDesignMode::SetPlayerController(APlayerController* Controller) {
	//To-Do
}

void UInteriorDesignMode::SetupInputComponent() {
	//To-Do
}
