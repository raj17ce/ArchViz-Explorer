// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingSubModes/RoofSubMode.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ArchVizUtility.h"
#include "ArchVizActors/BuildingActors/WallActor.h"

void URoofSubMode::Setup() {
	bNewRoofStart = false;
	CurrentRoofActor = nullptr;
	SubModeState = EBuildingSubModeState::Free;
}

void URoofSubMode::Cleanup() {
	if (IsValid(CurrentRoofActor)) {
		if ((CurrentRoofActor->GetState() == EBuildingActorState::Preview) || (CurrentRoofActor->GetState() == EBuildingActorState::Generating)) {
			CurrentRoofActor->Destroy();
		}
		else if (CurrentRoofActor->GetState() == EBuildingActorState::Moving) {
			CurrentRoofActor->SetState(EBuildingActorState::Selected);
		}
	}
}

void URoofSubMode::EnterSubMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->AddMappingContext(MappingContext, 0);

			Setup();
		}
	}
}

void URoofSubMode::ExitSubMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->RemoveMappingContext(MappingContext);
			Cleanup();
		}
	}
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

			//R-Key
			auto* RKeyPressAction = NewObject<UInputAction>(this);
			RKeyPressAction->ValueType = EInputActionValueType::Boolean;

			MappingContext->MapKey(RKeyPressAction, EKeys::R);
			EIC->BindAction(RKeyPressAction, ETriggerEvent::Completed, this, &URoofSubMode::HandleRKeyPress);

			//M-Key
			auto* MKeyPressAction = NewObject<UInputAction>(this);
			MKeyPressAction->ValueType = EInputActionValueType::Boolean;

			MappingContext->MapKey(MKeyPressAction, EKeys::M);
			EIC->BindAction(MKeyPressAction, ETriggerEvent::Completed, this, &URoofSubMode::HandleMKeyPress);
		}
	}
}

void URoofSubMode::HandleLeftMouseClick() {
	if (IsValid(RoofActorClass)) {

		switch (SubModeState) {
		case EBuildingSubModeState::Free:
			HandleFreeState();
			break;
		case EBuildingSubModeState::OldObject:
			HandleOldObjectState();
			break;
		case EBuildingSubModeState::NewObject:
			HandleNewObjectState();
			break;
		}
	}
}

void URoofSubMode::HandleRKeyPress() {
	if (IsValid(CurrentRoofActor)) {
		CurrentRoofActor->RotateActor(90.0);
	}
}

void URoofSubMode::HandleMKeyPress() {
	if (IsValid(CurrentRoofActor)) {
		CurrentRoofActor->SetState(EBuildingActorState::Moving);
		SubModeState = EBuildingSubModeState::OldObject;
	}
}

void URoofSubMode::HandleFreeState() {
	FHitResult HitResult = GetHitResult();

	if (HitResult.GetActor() && HitResult.GetActor()->IsA(ARoofActor::StaticClass())) {
		CurrentRoofActor = Cast<ARoofActor>(HitResult.GetActor());
		CurrentRoofActor->SetState(EBuildingActorState::Selected);
		//To-Do Display Widget
	}
	else {
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		CurrentRoofActor = GetWorld()->SpawnActor<ARoofActor>(RoofActorClass, SpawnParams);
		CurrentRoofActor->GenerateRoof(FVector{ 100.0,100.0, 20.0 }, FVector{ 50.0,50.0, 10.0 });
		CurrentRoofActor->SetState(EBuildingActorState::Preview);
		SubModeState = EBuildingSubModeState::NewObject;
		//To-Do Preview Material
	}
}

void URoofSubMode::HandleOldObjectState() {
	SubModeState = EBuildingSubModeState::Free;
	CurrentRoofActor->SetState(EBuildingActorState::Selected);
}

void URoofSubMode::HandleNewObjectState() {
	if (IsValid(CurrentRoofActor)) {

		FHitResult HitResult = GetHitResult(TArray<AActor*> {CurrentRoofActor});
		HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

		if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(AWallActor::StaticClass())) {
			if (!bNewRoofStart) {
				bNewRoofStart = true;
				CurrentRoofActor->SetActorLocation(HitResult.Location);
				CurrentRoofActor->SetStartPoint(HitResult.Location);
				CurrentRoofActor->SetState(EBuildingActorState::Generating);
			}
			else {
				bNewRoofStart = false;
				CurrentRoofActor->SetEndPoint(HitResult.Location);
				CurrentRoofActor->SetState(EBuildingActorState::Selected);
				SubModeState = EBuildingSubModeState::Free;
			}
		}
		else {
			//To-Do Notification
		}
	}
}
