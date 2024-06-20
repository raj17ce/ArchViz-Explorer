// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingSubModes/WallSubMode.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

void UWallSubMode::Setup() {
	if (WallActorClass) {
		WallActor = NewObject<AWallActor>(this, WallActorClass);
	}
}

void UWallSubMode::EnterSubMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

void UWallSubMode::ExitSubMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->RemoveMappingContext(MappingContext);
		}
	}
}

void UWallSubMode::SetPlayerController(APlayerController* Controller) {
	PlayerController = Controller;
}

void UWallSubMode::SetupInputComponent() {
	if (IsValid(PlayerController)) {
		if (auto* EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {

			MappingContext = NewObject<UInputMappingContext>();

			//Left-Click
			auto* LeftMouseClickAction = NewObject<UInputAction>();
			LeftMouseClickAction->ValueType = EInputActionValueType::Boolean;

			MappingContext->MapKey(LeftMouseClickAction, EKeys::LeftMouseButton);
			EIC->BindAction(LeftMouseClickAction, ETriggerEvent::Completed, this, &UWallSubMode::HandleLeftMouseClick);

			//R-Key
			auto* RKeyPressAction = NewObject<UInputAction>(this);
			RKeyPressAction->ValueType = EInputActionValueType::Boolean;

			MappingContext->MapKey(RKeyPressAction, EKeys::R);
			EIC->BindAction(RKeyPressAction, ETriggerEvent::Completed, this, &UWallSubMode::HandleRKeyPress);
		}
	}
}

void UWallSubMode::HandleLeftMouseClick() {

	FHitResult HitResult{};
	PlayerController->GetHitResultUnderCursorByChannel(TraceTypeQuery1, true, HitResult);

	if (IsValid(WallActor) && IsValid(WallActor->WallStaticMesh)) {
		auto* NewWallSegment = NewObject<UStaticMeshComponent>();
		NewWallSegment->SetStaticMesh(WallActor->WallStaticMesh);

		NewWallSegment->RegisterComponentWithWorld(GetWorld());
		NewWallSegment->SetWorldLocation(HitResult.Location);
		//NewWallSegment->SetWorldLocation(WallActor->PreviewWallSegment->GetComponentLocation());
		NewWallSegment->SetWorldRotation(WallActor->GetSegmentRotation());

		WallActor->WallSegments.Add(NewWallSegment);
		WallActor->SetSegmentIndex(WallActor->GetSegmentIndex() + 1);
	}
}

void UWallSubMode::HandleRKeyPress() {
	if (IsValid(WallActor)) {
		double NewYaw = (WallActor->GetSegmentRotation().Yaw + 90);
		if (NewYaw >= 360) {
			NewYaw -= 360;
		}
		WallActor->SetSegmentRotation(FRotator{ 0.0, NewYaw, 0.0 });
	}
}

AWallActor* UWallSubMode::GetWallActor() const {
	return WallActor;
}