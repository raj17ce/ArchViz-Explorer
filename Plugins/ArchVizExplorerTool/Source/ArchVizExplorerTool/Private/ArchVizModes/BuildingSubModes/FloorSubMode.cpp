// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingSubModes/FloorSubMode.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ArchVizUtility.h"

void UFloorSubMode::Setup() {
	bNewFloorStart = false;
	CurrentFloorActor = nullptr;
	SubModeState = EBuildingSubModeState::Free;
}

void UFloorSubMode::Cleanup() {
	if (IsValid(CurrentFloorActor)) {
		if ((CurrentFloorActor->GetState() == EBuildingActorState::Preview) || (CurrentFloorActor->GetState() == EBuildingActorState::Generating)) {
			CurrentFloorActor->Destroy();
		}
		else if (CurrentFloorActor->GetState() == EBuildingActorState::Moving) {
			CurrentFloorActor->SetState(EBuildingActorState::Selected);
		}
	}
}

void UFloorSubMode::EnterSubMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->AddMappingContext(MappingContext, 0);

			Setup();
		}
	}
}

void UFloorSubMode::ExitSubMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->RemoveMappingContext(MappingContext);
			Cleanup();
		}
	}
}

void UFloorSubMode::SetupInputComponent() {
	if (IsValid(PlayerController)) {
		if (auto* EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {

			MappingContext = NewObject<UInputMappingContext>();

			//Left-Click
			auto* LeftMouseClickAction = NewObject<UInputAction>();
			LeftMouseClickAction->ValueType = EInputActionValueType::Boolean;

			MappingContext->MapKey(LeftMouseClickAction, EKeys::LeftMouseButton);
			EIC->BindAction(LeftMouseClickAction, ETriggerEvent::Completed, this, &UFloorSubMode::HandleLeftMouseClick);

			//R-Key
			auto* RKeyPressAction = NewObject<UInputAction>(this);
			RKeyPressAction->ValueType = EInputActionValueType::Boolean;

			MappingContext->MapKey(RKeyPressAction, EKeys::R);
			EIC->BindAction(RKeyPressAction, ETriggerEvent::Completed, this, &UFloorSubMode::HandleRKeyPress);

			//M-Key
			auto* MKeyPressAction = NewObject<UInputAction>(this);
			MKeyPressAction->ValueType = EInputActionValueType::Boolean;

			MappingContext->MapKey(MKeyPressAction, EKeys::M);
			EIC->BindAction(MKeyPressAction, ETriggerEvent::Completed, this, &UFloorSubMode::HandleMKeyPress);
		}
	}
}

void UFloorSubMode::HandleLeftMouseClick() {
	if (IsValid(FloorActorClass)) {

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

void UFloorSubMode::HandleRKeyPress() {
	if (IsValid(CurrentFloorActor)) {
		CurrentFloorActor->RotateActor(90.0);
	}
}

void UFloorSubMode::HandleMKeyPress() {
	if (IsValid(CurrentFloorActor)) {
		CurrentFloorActor->SetState(EBuildingActorState::Moving);
		SubModeState = EBuildingSubModeState::OldObject;
	}
}

void UFloorSubMode::HandleFreeState() {
	FHitResult HitResult = GetHitResult();

	if (HitResult.GetActor() && HitResult.GetActor()->IsA(AFloorActor::StaticClass())) {
		CurrentFloorActor = Cast<AFloorActor>(HitResult.GetActor());
		CurrentFloorActor->SetState(EBuildingActorState::Selected);
		//To-Do Display Widget
	}
	else {
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		CurrentFloorActor = GetWorld()->SpawnActor<AFloorActor>(FloorActorClass, SpawnParams);
		CurrentFloorActor->GenerateFloor(FVector{ 100.0,100.0, 2.0 }, FVector{ 50.0,50.0, 1.0});
		CurrentFloorActor->SetState(EBuildingActorState::Preview);
		SubModeState = EBuildingSubModeState::NewObject;
		//To-Do Preview Material
	}
}

void UFloorSubMode::HandleOldObjectState() {
	SubModeState = EBuildingSubModeState::Free;
	CurrentFloorActor->SetState(EBuildingActorState::Selected);
}

void UFloorSubMode::HandleNewObjectState() {
	if (IsValid(CurrentFloorActor)) {

		FHitResult HitResult = GetHitResult(TArray<AActor*> {CurrentFloorActor});
		HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

		if (!bNewFloorStart) {
			bNewFloorStart = true;
			CurrentFloorActor->SetActorLocation(HitResult.Location);
			CurrentFloorActor->SetStartPoint(HitResult.Location);
			CurrentFloorActor->SetState(EBuildingActorState::Generating);
		}
		else {
			bNewFloorStart = false;
			CurrentFloorActor->SetEndPoint(HitResult.Location);
			CurrentFloorActor->SetState(EBuildingActorState::Selected);
			SubModeState = EBuildingSubModeState::Free;
		}
	}
}
