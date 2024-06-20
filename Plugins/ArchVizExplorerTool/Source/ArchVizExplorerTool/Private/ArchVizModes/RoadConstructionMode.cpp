// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/RoadConstructionMode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void URoadConstructionMode::EnterMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

void URoadConstructionMode::ExitMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->RemoveMappingContext(MappingContext);
		}
	}
}

void URoadConstructionMode::SetPlayerController(APlayerController* Controller) {
	PlayerController = Controller;
}

void URoadConstructionMode::SetupInputComponent() {
	if (auto* EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {

		MappingContext = NewObject<UInputMappingContext>();

		//Left-Click
		auto* LeftMouseClickAction = NewObject<UInputAction>();
		LeftMouseClickAction->ValueType = EInputActionValueType::Boolean;

		MappingContext->MapKey(LeftMouseClickAction, EKeys::LeftMouseButton);
		EIC->BindAction(LeftMouseClickAction, ETriggerEvent::Completed, this, &URoadConstructionMode::HandleLeftMouseClick);
	}
}

void URoadConstructionMode::HandleLeftMouseClick() {
	FHitResult HitResult{};
	PlayerController->GetHitResultUnderCursorByChannel(TraceTypeQuery1, true, HitResult);

	if (IsValid(RoadActorClass) && !IsValid(RoadActor)) {
		RoadActor = NewObject<ARoadActor>(this, RoadActorClass);
	}

	RoadActor->AddSplinePoint(HitResult.Location);
}