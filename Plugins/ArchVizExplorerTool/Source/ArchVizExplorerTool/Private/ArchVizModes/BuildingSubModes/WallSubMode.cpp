// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingSubModes/WallSubMode.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ArchVizUtility.h"
#include "ArchVizActors/BuildingActors/DoorActor.h"
#include "ArchVizActors/BuildingActors/FloorActor.h"
#include "ArchVizActors/BuildingActors/RoofActor.h"

void UWallSubMode::Setup() {
	bNewWallStart = false;
	CurrentWallActor = nullptr;
	SubModeState = EBuildingSubModeState::Free;
}

void UWallSubMode::Cleanup() {
	if (IsValid(CurrentWallActor)) {
		if ((CurrentWallActor->GetState() == EBuildingActorState::Preview) || (CurrentWallActor->GetState() == EBuildingActorState::Generating)) {
			CurrentWallActor->SetState(EBuildingActorState::None);
			CurrentWallActor->DestroyDoorComponents();
			CurrentWallActor->Destroy();
		}
		else {
			CurrentWallActor->SetState(EBuildingActorState::None);
		}
		CurrentWallActor = nullptr;
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

			//Delete-Key
			auto* DeleteKeyPressAction = NewObject<UInputAction>(this);
			DeleteKeyPressAction->ValueType = EInputActionValueType::Boolean;

			MappingContext->MapKey(DeleteKeyPressAction, EKeys::Delete);
			EIC->BindAction(DeleteKeyPressAction, ETriggerEvent::Completed, this, &UWallSubMode::HandleWallDeleteButtonClick);
		}
	}
}

void UWallSubMode::SelectActor(AWallActor* WallActor) {
	if (IsValid(CurrentWallActor)) {
		CurrentWallActor->SetState(EBuildingActorState::None);
		CurrentWallActor = nullptr;
	}

	CurrentWallActor = WallActor;

	if (IsValid(CurrentWallActor)) {
		CurrentWallActor->SetState(EBuildingActorState::Selected);
	}
}

void UWallSubMode::HandleLeftMouseClick() {

	if (IsValid(WallActorClass)) {
	
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

void UWallSubMode::HandleRKeyPress() {
	if (IsValid(CurrentWallActor)) {
		CurrentWallActor->RotateActor(90.0);
		CurrentWallActor->HandleEdgeOffset();
	}
}

void UWallSubMode::HandleMKeyPress() {
	if (IsValid(CurrentWallActor) && CurrentWallActor->GetState() == EBuildingActorState::Selected) {
		CurrentWallActor->SetState(EBuildingActorState::Moving);
		SubModeState = EBuildingSubModeState::OldObject;
	}
}

void UWallSubMode::HandleFreeState() {
	FHitResult HitResult = GetHitResult();

	if (IsValid(CurrentWallActor)) {
		CurrentWallActor->SetState(EBuildingActorState::None);
		CurrentWallActor = nullptr;
	}
	

	if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(AWallActor::StaticClass())) {
		CurrentWallActor = Cast<AWallActor>(HitResult.GetActor());
		CurrentWallActor->SetState(EBuildingActorState::Selected);
	}
	else if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(ADoorActor::StaticClass())) {
		OnOtherBuildingActorSelected.ExecuteIfBound(EBuildingSubMode::DoorConstruction, HitResult.GetActor());
	}
	else if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(AFloorActor::StaticClass())) {
		OnOtherBuildingActorSelected.ExecuteIfBound(EBuildingSubMode::FloorConstruction, HitResult.GetActor());
	}
	else if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(ARoofActor::StaticClass())) {
		OnOtherBuildingActorSelected.ExecuteIfBound(EBuildingSubMode::RoofConstruction, HitResult.GetActor());
	}
	else {
		if (IsValid(WallActorClass)) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			CurrentWallActor = GetWorld()->SpawnActor<AWallActor>(WallActorClass, SpawnParams);
			BindWidgetDelegates();
			CurrentWallActor->GenerateWallSegments();
			CurrentWallActor->SetState(EBuildingActorState::Preview);
			SubModeState = EBuildingSubModeState::NewObject;
			//To-Do Preview Material
		}
	}
}

void UWallSubMode::HandleOldObjectState() {
	if (IsValid(CurrentWallActor)) {
		SubModeState = EBuildingSubModeState::Free;
		CurrentWallActor->SetState(EBuildingActorState::Selected);
	}
}

void UWallSubMode::HandleNewObjectState() {
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
			CurrentWallActor->UpdateLengthSpinBoxValue();
			CurrentWallActor->SetState(EBuildingActorState::Selected);
			SubModeState = EBuildingSubModeState::Free;
		}
	}
}

void UWallSubMode::BindWidgetDelegates() {
	if (IsValid(CurrentWallActor) && IsValid(CurrentWallActor->PropertyPanelWidget)) {
		CurrentWallActor->PropertyPanelWidget->WallNewButton->OnClicked.AddDynamic(this, &UWallSubMode::HandleWallNewButtonClick);
		CurrentWallActor->PropertyPanelWidget->WallDeleteButton->OnClicked.AddDynamic(this, &UWallSubMode::HandleWallDeleteButtonClick);
		CurrentWallActor->PropertyPanelWidget->WallCloseButton->OnClicked.AddDynamic(this, &UWallSubMode::HandleWallCloseButtonClick);
		CurrentWallActor->PropertyPanelWidget->WallLengthSpinbox->OnValueChanged.AddDynamic(this, &UWallSubMode::HandleWallLengthSpinBoxValueChange);
	}
}

void UWallSubMode::HandleWallLengthSpinBoxValueChange(float InLength) {
	if (IsValid(CurrentWallActor)) {
		CurrentWallActor->SetLength(InLength);
		CurrentWallActor->GenerateWallSegments();
	}
}

void UWallSubMode::HandleWallNewButtonClick() {
	if (IsValid(CurrentWallActor)) {
		CurrentWallActor->SetState(EBuildingActorState::None);
		CurrentWallActor = nullptr;

		if (IsValid(WallActorClass)) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			CurrentWallActor = GetWorld()->SpawnActor<AWallActor>(WallActorClass, SpawnParams);
			BindWidgetDelegates();
			CurrentWallActor->GenerateWallSegments();
			CurrentWallActor->SetState(EBuildingActorState::Preview);
			SubModeState = EBuildingSubModeState::NewObject;
		}
	}
}

void UWallSubMode::HandleWallDeleteButtonClick() {
	if (IsValid(CurrentWallActor) && CurrentWallActor->GetState() == EBuildingActorState::Selected) {
		CurrentWallActor->SetState(EBuildingActorState::None);
		CurrentWallActor->DestroyDoorComponents();
		CurrentWallActor->Destroy();
		CurrentWallActor = nullptr;
	}
}

void UWallSubMode::HandleWallCloseButtonClick() {
	if (IsValid(CurrentWallActor)) {
		CurrentWallActor->SetState(EBuildingActorState::None);
		CurrentWallActor = nullptr;
	}
}
