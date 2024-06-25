// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingSubModes/DoorSubMode.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ArchVizUtility.h"
#include "ArchVizActors/BuildingActors/WallActor.h"

void UDoorSubMode::Setup() {
	CurrentDoorActor = nullptr;
	SubModeState = EBuildingSubModeState::Free;
}

void UDoorSubMode::Cleanup() {
	if (IsValid(CurrentDoorActor)) {
		if ((CurrentDoorActor->GetState() == EBuildingActorState::Preview)) {
			CurrentDoorActor->Destroy();
		}
		else if (CurrentDoorActor->GetState() == EBuildingActorState::Moving) {
			CurrentDoorActor->SetState(EBuildingActorState::Selected);
		}
	}
}

void UDoorSubMode::EnterSubMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->AddMappingContext(MappingContext, 0);

			Setup();
		}
	}
}

void UDoorSubMode::ExitSubMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->RemoveMappingContext(MappingContext);

			Cleanup();
		}
	}
}

void UDoorSubMode::SetupInputComponent() {
	if (IsValid(PlayerController)) {
		if (auto* EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {

			MappingContext = NewObject<UInputMappingContext>();

			//Left-Click
			auto* LeftMouseClickAction = NewObject<UInputAction>();
			LeftMouseClickAction->ValueType = EInputActionValueType::Boolean;

			MappingContext->MapKey(LeftMouseClickAction, EKeys::LeftMouseButton);
			EIC->BindAction(LeftMouseClickAction, ETriggerEvent::Completed, this, &UDoorSubMode::HandleLeftMouseClick);

			//R-Key
			auto* RKeyPressAction = NewObject<UInputAction>(this);
			RKeyPressAction->ValueType = EInputActionValueType::Boolean;

			MappingContext->MapKey(RKeyPressAction, EKeys::R);
			EIC->BindAction(RKeyPressAction, ETriggerEvent::Completed, this, &UDoorSubMode::HandleRKeyPress);

			//M-Key
			auto* MKeyPressAction = NewObject<UInputAction>(this);
			MKeyPressAction->ValueType = EInputActionValueType::Boolean;

			MappingContext->MapKey(MKeyPressAction, EKeys::M);
			EIC->BindAction(MKeyPressAction, ETriggerEvent::Completed, this, &UDoorSubMode::HandleMKeyPress);

			//O-Key
			auto* OKeyPressAction = NewObject<UInputAction>(this);
			OKeyPressAction->ValueType = EInputActionValueType::Boolean;

			MappingContext->MapKey(OKeyPressAction, EKeys::O);
			EIC->BindAction(OKeyPressAction, ETriggerEvent::Completed, this, &UDoorSubMode::HandleOKeyPress);
		}
	}
}

void UDoorSubMode::HandleLeftMouseClick() {
	if (IsValid(DoorActorClass)) {

		switch (SubModeState) {
		case EBuildingSubModeState::Free:
		{
			FHitResult HitResult = GetHitResult();
			//HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

			if (HitResult.GetActor() && HitResult.GetActor()->IsA(ADoorActor::StaticClass())) {
				CurrentDoorActor = Cast<ADoorActor>(HitResult.GetActor());
				CurrentDoorActor->SetState(EBuildingActorState::Selected);
				//To-Do Display Widget
			}
			else {
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				CurrentDoorActor = GetWorld()->SpawnActor<ADoorActor>(DoorActorClass, SpawnParams);
				CurrentDoorActor->SetState(EBuildingActorState::Preview);
				SubModeState = EBuildingSubModeState::NewObject;
				//To-Do Preview Material
			}
		}
			break;
		case EBuildingSubModeState::OldObject:
			SubModeState = EBuildingSubModeState::Free;
			CurrentDoorActor->SetState(EBuildingActorState::Selected);
			break;
		case EBuildingSubModeState::NewObject:
			if (IsValid(CurrentDoorActor)) {

				FHitResult HitResult = GetHitResult(TArray<AActor*> {CurrentDoorActor});
				//HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

				if (HitResult.GetActor() && HitResult.GetActor()->IsA(AWallActor::StaticClass())) {
					AWallActor* WallActor = Cast<AWallActor>(HitResult.GetActor());

					if (IsValid(WallActor) && HitResult.GetComponent()) {
						CurrentDoorActor->SetState(EBuildingActorState::Selected);
						WallActor->AttachDoorComponent(HitResult.GetComponent(), CurrentDoorActor);
						SubModeState = EBuildingSubModeState::Free;
					}
				}
			}
			break;
		}
	}
}

void UDoorSubMode::HandleRKeyPress() {
	if (IsValid(CurrentDoorActor)) {
		CurrentDoorActor->RotateActor(90.0);
	}
}

void UDoorSubMode::HandleMKeyPress() {
	if (IsValid(CurrentDoorActor)) {
		CurrentDoorActor->SetState(EBuildingActorState::Moving);
		SubModeState = EBuildingSubModeState::OldObject;
	}
}

void UDoorSubMode::HandleOKeyPress() {
	if (IsValid(CurrentDoorActor)) {
		if(CurrentDoorActor->DoorComponent->GetRelativeRotation().Yaw == 0.0) {
			CurrentDoorActor->DoorComponent->SetRelativeRotation(FRotator{0.0, 90.0, 0.0});
		}
		else {
			CurrentDoorActor->DoorComponent->SetRelativeRotation(FRotator{ 0.0 });
		}
	}
}