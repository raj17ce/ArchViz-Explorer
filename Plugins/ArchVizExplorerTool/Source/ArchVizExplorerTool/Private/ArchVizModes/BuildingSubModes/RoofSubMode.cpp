// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingSubModes/RoofSubMode.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

void URoofSubMode::Setup() {
	//To-Do
}

void URoofSubMode::EnterSubMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

void URoofSubMode::ExitSubMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->RemoveMappingContext(MappingContext);
		}
	}
}

void URoofSubMode::SetPlayerController(APlayerController* Controller) {
	PlayerController = Controller;
}

void URoofSubMode::SetupInputComponent() {
	if (IsValid(PlayerController)) {
		if (auto* EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {

			MappingContext = NewObject<UInputMappingContext>();

			//Left-Click
			auto* LeftMouseClickAction = NewObject<UInputAction>();
			LeftMouseClickAction->ValueType = EInputActionValueType::Boolean;

			MappingContext->MapKey(LeftMouseClickAction, EKeys::LeftMouseButton);
			EIC->BindAction(LeftMouseClickAction, ETriggerEvent::Completed, this, &URoofSubMode::HandleLeftMouseClick);
		}
	}
}

void URoofSubMode::HandleLeftMouseClick() {
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, "Roof Left Clicked");
}
