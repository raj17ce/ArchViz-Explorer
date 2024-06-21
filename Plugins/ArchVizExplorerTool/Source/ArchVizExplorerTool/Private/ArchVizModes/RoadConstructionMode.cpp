// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/RoadConstructionMode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void URoadConstructionMode::Setup() {
	if (IsValid(RoadActorClass) && !IsValid(RoadActor)) {
		RoadActor = GetWorld()->SpawnActor<ARoadActor>(RoadActorClass, FTransform {});
	}

	if (IsValid(WidgetClass) && !IsValid(Widget)) {
		Widget = CreateWidget<URoadConstructionWidget>(GetWorld(), WidgetClass, "Road Widget");
	}
}

void URoadConstructionMode::EnterMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->AddMappingContext(MappingContext, 0);
			ShowWidget();
		}
	}
}

void URoadConstructionMode::ExitMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->RemoveMappingContext(MappingContext);
			HideWidget();
		}
	}
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
	if (IsValid(RoadActor)) {
		FHitResult HitResult{};

		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(RoadActor);

		HitResult = RoadActor->GetHitResult(IgnoredActors);
		RoadActor->AddSplinePoint(HitResult.Location);
	}
}