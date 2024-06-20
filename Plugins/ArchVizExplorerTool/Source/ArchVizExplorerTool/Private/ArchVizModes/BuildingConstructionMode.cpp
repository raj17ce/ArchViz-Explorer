// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingConstructionMode.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/StaticMeshComponent.h"

void UBuildingConstructionMode::EnterMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->AddMappingContext(MappingContext, 0);
		}

		if (IsValid(WallActorClass) && !IsValid(WallActor)) {
			WallActor = NewObject<AWallActor>(this, WallActorClass);
		}
	}
}

void UBuildingConstructionMode::ExitMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->RemoveMappingContext(MappingContext);
		}
	}
}

void UBuildingConstructionMode::SetPlayerController(APlayerController* Controller) {
	PlayerController = Controller;
}

void UBuildingConstructionMode::SetupInputComponent() {
	if (auto* EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {

		MappingContext = NewObject<UInputMappingContext>();

		//Left-Click
		auto* LeftMouseClickAction = NewObject<UInputAction>();
		LeftMouseClickAction->ValueType = EInputActionValueType::Boolean;

		MappingContext->MapKey(LeftMouseClickAction, EKeys::LeftMouseButton);
		EIC->BindAction(LeftMouseClickAction, ETriggerEvent::Completed, this, &UBuildingConstructionMode::HandleLeftMouseClick);

		//R-Key
		auto* RKeyPressAction = NewObject<UInputAction>(this);
		RKeyPressAction->ValueType = EInputActionValueType::Boolean;

		MappingContext->MapKey(RKeyPressAction, EKeys::R);
		EIC->BindAction(RKeyPressAction, ETriggerEvent::Completed, this, &UBuildingConstructionMode::HandleRKeyPress);
	}
}

void UBuildingConstructionMode::HandleLeftMouseClick() {
	auto* NewWallSegment = NewObject<UStaticMeshComponent>();

	if (IsValid(WallActor) && IsValid(WallActor->WallStaticMesh)) {
		NewWallSegment->SetStaticMesh(WallActor->WallStaticMesh);

		NewWallSegment->RegisterComponentWithWorld(GetWorld());
		NewWallSegment->SetWorldLocation(WallActor->PreviewWallSegment->GetComponentLocation());
		NewWallSegment->SetWorldRotation(WallActor->GetSegmentRotation());

		WallActor->WallSegments.Add(NewWallSegment);
		WallActor->SetSegmentIndex(WallActor->GetSegmentIndex() + 1);
	}
}

void UBuildingConstructionMode::HandleRKeyPress() {
	if (IsValid(WallActor)) {
		double NewYaw = (WallActor->GetSegmentRotation().Yaw + 90);
		if (NewYaw >= 360) {
			NewYaw -= 360;
		}
		WallActor->SetSegmentRotation(FRotator{ 0.0, NewYaw, 0.0 });
	}
}

AWallActor* UBuildingConstructionMode::GetWallActor() const {
	return WallActor;
}
