// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingSubModes/WallSubMode.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ArchVizUtility.h"

void UWallSubMode::Setup() {
	bNewWallStart = false;
	CurrentWallActor = nullptr;
	SubModeState = EBuildingSubModeState::Free;
}

void UWallSubMode::Cleanup() {
	if (IsValid(CurrentWallActor)) {
		if ((CurrentWallActor->GetState() == EBuildingActorState::Preview) || (CurrentWallActor->GetState() == EBuildingActorState::Generating)) {
			CurrentWallActor->Destroy();
		}
		else if (CurrentWallActor->GetState() == EBuildingActorState::Moving) {
			CurrentWallActor->SetState(EBuildingActorState::Selected);
		}
	}
}

void UWallSubMode::EnterSubMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->AddMappingContext(MappingContext, 0);

			Setup();
		}
	}
}

void UWallSubMode::ExitSubMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->RemoveMappingContext(MappingContext);

			Cleanup();
		}
	}
}

void UWallSubMode::SetupInputComponent() {
	if (IsValid(PlayerController)) {
		if (auto* EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {

			MappingContext = NewObject<UInputMappingContext>(this);

			//Left-Click
			auto* LeftMouseClickAction = NewObject<UInputAction>(this);
			LeftMouseClickAction->ValueType = EInputActionValueType::Boolean;

			MappingContext->MapKey(LeftMouseClickAction, EKeys::LeftMouseButton);
			EIC->BindAction(LeftMouseClickAction, ETriggerEvent::Completed, this, &UWallSubMode::HandleLeftMouseClick);

			//R-Key
			auto* RKeyPressAction = NewObject<UInputAction>(this);
			RKeyPressAction->ValueType = EInputActionValueType::Boolean;

			MappingContext->MapKey(RKeyPressAction, EKeys::R);
			EIC->BindAction(RKeyPressAction, ETriggerEvent::Completed, this, &UWallSubMode::HandleRKeyPress);

			//M-Key
			auto* MKeyPressAction = NewObject<UInputAction>(this);
			MKeyPressAction->ValueType = EInputActionValueType::Boolean;

			MappingContext->MapKey(MKeyPressAction, EKeys::M);
			EIC->BindAction(MKeyPressAction, ETriggerEvent::Completed, this, &UWallSubMode::HandleMKeyPress);
		}
	}
}

void UWallSubMode::HandleLeftMouseClick() {

	if (IsValid(WallActorClass)) {
	
		switch (SubModeState) {
		case EBuildingSubModeState::Free:
		{
			FHitResult HitResult = GetHitResult();
			HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

			if (HitResult.GetActor() && HitResult.GetActor()->IsA(AWallActor::StaticClass())) {
				CurrentWallActor = Cast<AWallActor>(HitResult.GetActor());
				CurrentWallActor->SetState(EBuildingActorState::Selected);
				//To-Do Display Widget
			}
			else {
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				CurrentWallActor = GetWorld()->SpawnActor<AWallActor>(WallActorClass, SpawnParams);
				CurrentWallActor->GenerateWallSegments();
				CurrentWallActor->SetState(EBuildingActorState::Preview);
				SubModeState = EBuildingSubModeState::NewObject;
				//To-Do Preview Material
			}
		}
			break;
		case EBuildingSubModeState::OldObject:
			SubModeState = EBuildingSubModeState::Free;
			CurrentWallActor->SetState(EBuildingActorState::Selected);
			break;
		case EBuildingSubModeState::NewObject:
			if (IsValid(CurrentWallActor)) {

				FHitResult HitResult = GetHitResult(TArray<AActor*> {CurrentWallActor});
				HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

				if (!bNewWallStart) {
					bNewWallStart = true;

					CurrentWallActor->SetActorLocation(HitResult.Location);
					CurrentWallActor->SetStartLocation(HitResult.Location);
					CurrentWallActor->SetState(EBuildingActorState::Generating);
				}
				else {
					bNewWallStart = false;
					CurrentWallActor->SetEndLocation(HitResult.Location);
					CurrentWallActor->SetState(EBuildingActorState::Selected);
					SubModeState = EBuildingSubModeState::Free;
				}
			}
			break;
		}
	}
}

void UWallSubMode::HandleRKeyPress() {
	if (IsValid(CurrentWallActor)) {
		CurrentWallActor->RotateActor(90.0);
		CurrentWallActor->HandleEdgeOffset();
	}
}

void UWallSubMode::HandleMKeyPress() {
	if (IsValid(CurrentWallActor)) {
		CurrentWallActor->SetState(EBuildingActorState::Moving);
		SubModeState = EBuildingSubModeState::OldObject;
	}
}