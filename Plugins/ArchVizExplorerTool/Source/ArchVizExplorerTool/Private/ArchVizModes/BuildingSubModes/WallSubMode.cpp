// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingSubModes/WallSubMode.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ArchVizUtility.h"

void UWallSubMode::Setup() {
	bIsFirstClick = false;
	CurrentWallActor = nullptr;
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
	if (IsValid(WallActorClass)) {
		if (!bIsFirstClick) {
			bIsFirstClick = true;

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			CurrentWallActor = GetWorld()->SpawnActor<AWallActor>(WallActorClass, SpawnParams);

			FHitResult HitResult = CurrentWallActor->GetHitResult(TArray<AActor*>{CurrentWallActor});
			HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

			CurrentWallActor->SetActorLocation(HitResult.Location);
			CurrentWallActor->SetStartLocation(HitResult.Location);
			CurrentWallActor->SetShowPreview(bIsFirstClick);
		}
		else {
			bIsFirstClick = false;

			FHitResult HitResult = CurrentWallActor->GetHitResult(TArray<AActor*>{CurrentWallActor});
			CurrentWallActor->SetEndLocation(HitResult.Location);
			CurrentWallActor->SetShowPreview(bIsFirstClick);
			//CurrentWallActor->GenerateWallSegments();
		}
	}
}

void UWallSubMode::HandleRKeyPress() {
	if (IsValid(CurrentWallActor)) {
		CurrentWallActor->RotateActor(90.0);
	}
}