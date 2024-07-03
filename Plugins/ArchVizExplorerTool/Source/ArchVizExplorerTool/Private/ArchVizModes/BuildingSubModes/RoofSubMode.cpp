// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingSubModes/RoofSubMode.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ArchVizUtility.h"
#include "ArchVizController.h"
#include "ArchVizActors/BuildingActors/WallActor.h"
#include "ArchVizActors/BuildingActors/DoorActor.h"
#include "ArchVizActors/BuildingActors/FloorActor.h"

void URoofSubMode::Setup() {
	bNewRoofStart = false;
	CurrentRoofActor = nullptr;
	SubModeState = EBuildingSubModeState::Free;
}

void URoofSubMode::Cleanup() {
	if (IsValid(CurrentRoofActor)) {
		if ((CurrentRoofActor->GetState() == EBuildingActorState::Preview) || (CurrentRoofActor->GetState() == EBuildingActorState::Generating)) {
			CurrentRoofActor->SetState(EBuildingActorState::None);
			CurrentRoofActor->Destroy();
		}
		else {
			CurrentRoofActor->SetState(EBuildingActorState::None);
		}
		CurrentRoofActor = nullptr;
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
		
			//Delete-Key
			auto* DeleteKeyPressAction = NewObject<UInputAction>(this);
			DeleteKeyPressAction->ValueType = EInputActionValueType::Boolean;

			MappingContext->MapKey(DeleteKeyPressAction, EKeys::Delete);
			EIC->BindAction(DeleteKeyPressAction, ETriggerEvent::Completed, this, &URoofSubMode::HandleRoofDeleteButtonClick);
		}
	}
}

void URoofSubMode::BindPropertyDelegatesToActor(ABuildingActor* Actor) {
	if (auto* RoofActor = Cast<ARoofActor>(Actor)) {
		if (IsValid(RoofActor) && IsValid(RoofActor->PropertyPanelWidget)) {
			RoofActor->PropertyPanelWidget->RoofNewButton->OnClicked.AddDynamic(this, &URoofSubMode::HandleRoofNewButtonClick);
			RoofActor->PropertyPanelWidget->RoofDeleteButton->OnClicked.AddDynamic(this, &URoofSubMode::HandleRoofDeleteButtonClick);
			RoofActor->PropertyPanelWidget->RoofCloseButton->OnClicked.AddDynamic(this, &URoofSubMode::HandleRoofCloseButtonClick);
			RoofActor->PropertyPanelWidget->RoofLengthSpinbox->OnValueChanged.AddDynamic(this, &URoofSubMode::HandleRoofSpinBoxValueChange);
			RoofActor->PropertyPanelWidget->RoofWidthSpinbox->OnValueChanged.AddDynamic(this, &URoofSubMode::HandleRoofSpinBoxValueChange);
			RoofActor->PropertyPanelWidget->RoofHeightSpinbox->OnValueChanged.AddDynamic(this, &URoofSubMode::HandleRoofSpinBoxValueChange);
		}
	}
}

void URoofSubMode::SelectActor(ARoofActor* RoofActor) {
	if (IsValid(CurrentRoofActor)) {
		CurrentRoofActor->SetState(EBuildingActorState::None);
		CurrentRoofActor = nullptr;
	}

	CurrentRoofActor = RoofActor;

	if (IsValid(CurrentRoofActor)) {
		CurrentRoofActor->SetState(EBuildingActorState::Selected);
		PlayerController->AddSuccessMessage(FText::FromString("Roof Selected Successfully"), 1.5f);
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
		PlayerController->AddSuccessMessage(FText::FromString("Roof Rotated Successfully"), 1.5f);
	}
}

void URoofSubMode::HandleMKeyPress() {
	if (IsValid(CurrentRoofActor) && CurrentRoofActor->GetState() == EBuildingActorState::Selected) {
		CurrentRoofActor->SetState(EBuildingActorState::Moving);
		SubModeState = EBuildingSubModeState::OldObject;
	}
}

void URoofSubMode::HandleFreeState() {
	FHitResult HitResult = GetHitResult();

	if (IsValid(CurrentRoofActor)) {
		CurrentRoofActor->SetState(EBuildingActorState::None);
		CurrentRoofActor = nullptr;
	}

	if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(ARoofActor::StaticClass())) {
		CurrentRoofActor = Cast<ARoofActor>(HitResult.GetActor());
		CurrentRoofActor->SetState(EBuildingActorState::Selected);
		PlayerController->AddSuccessMessage(FText::FromString("Roof Selected Successfully"), 1.5f);
	}
	else if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(AWallActor::StaticClass())) {
		OnOtherBuildingActorSelected.ExecuteIfBound(EBuildingSubMode::WallConstruction, HitResult.GetActor());
	}
	else if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(ADoorActor::StaticClass())) {
		OnOtherBuildingActorSelected.ExecuteIfBound(EBuildingSubMode::DoorConstruction, HitResult.GetActor());
	}
	else if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(AFloorActor::StaticClass())) {
		OnOtherBuildingActorSelected.ExecuteIfBound(EBuildingSubMode::FloorConstruction, HitResult.GetActor());
	}
	else {
		if (IsValid(RoofActorClass)) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			CurrentRoofActor = GetWorld()->SpawnActor<ARoofActor>(RoofActorClass, SpawnParams);
			BindPropertyDelegatesToActor(CurrentRoofActor);
			CurrentRoofActor->GenerateRoof();
			CurrentRoofActor->SetState(EBuildingActorState::Preview);
			SubModeState = EBuildingSubModeState::NewObject;
			PlayerController->AddSuccessMessage(FText::FromString("Roof Preview Successfully"), 1.5f);
		}
	}
}

void URoofSubMode::HandleOldObjectState() {
	if (IsValid(CurrentRoofActor)) {
		SubModeState = EBuildingSubModeState::Free;
		CurrentRoofActor->SetState(EBuildingActorState::Selected);
		PlayerController->AddSuccessMessage(FText::FromString("Roof Moved Successfully"), 1.5f);
	}
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
				PlayerController->AddSuccessMessage(FText::FromString("Roof Generation Started"), 1.5f);
			}
			else {
				bNewRoofStart = false;
				CurrentRoofActor->SetEndPoint(HitResult.Location);
				CurrentRoofActor->UpdateSpinBoxValue();
				CurrentRoofActor->SetState(EBuildingActorState::Selected);
				SubModeState = EBuildingSubModeState::Free;
				PlayerController->AddSuccessMessage(FText::FromString("Roof Generated Successfully"), 1.5f);
			}
		}
		else {
			PlayerController->AddErrorMessage(FText::FromString("Roof Can Only be Generated on a Wall"), 1.5f);
		}
	}
}

void URoofSubMode::HandleRoofSpinBoxValueChange(float InLength) {
	if (IsValid(CurrentRoofActor) && IsValid(CurrentRoofActor->PropertyPanelWidget)) {
		float Length = CurrentRoofActor->PropertyPanelWidget->RoofLengthSpinbox->GetValue();
		float Width = CurrentRoofActor->PropertyPanelWidget->RoofWidthSpinbox->GetValue();
		float Height = CurrentRoofActor->PropertyPanelWidget->RoofHeightSpinbox->GetValue();

		double EdgeOffset{ 10.0 };

		CurrentRoofActor->SetDimensions(FVector{ Length + (2 * EdgeOffset), Width + (2 * EdgeOffset), Height });
		CurrentRoofActor->SetOffset(FVector{ Length / 2 , Width / 2, Height / 2 });
		CurrentRoofActor->GenerateRoof();
		PlayerController->AddSuccessMessage(FText::FromString("Roof Updated Successfully"), 1.5f);
	}
}

void URoofSubMode::HandleRoofNewButtonClick() {
	if (IsValid(CurrentRoofActor)) {
		CurrentRoofActor->SetState(EBuildingActorState::None);
		CurrentRoofActor = nullptr;

		if (IsValid(RoofActorClass)) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			CurrentRoofActor = GetWorld()->SpawnActor<ARoofActor>(RoofActorClass, SpawnParams);
			BindPropertyDelegatesToActor(CurrentRoofActor);
			CurrentRoofActor->GenerateRoof();
			CurrentRoofActor->SetState(EBuildingActorState::Preview);
			SubModeState = EBuildingSubModeState::NewObject;
			PlayerController->AddSuccessMessage(FText::FromString("Roof Preview Started"), 1.5f);
		}
	}
}

void URoofSubMode::HandleRoofDeleteButtonClick() {
	if (IsValid(CurrentRoofActor) && CurrentRoofActor->GetState() == EBuildingActorState::Selected) {
		CurrentRoofActor->SetState(EBuildingActorState::None);
		CurrentRoofActor->Destroy();
		CurrentRoofActor = nullptr;
		PlayerController->AddSuccessMessage(FText::FromString("Roof Deleted Successfully"), 1.5f);
	}
}

void URoofSubMode::HandleRoofCloseButtonClick() {
	if (IsValid(CurrentRoofActor)) {
		CurrentRoofActor->SetState(EBuildingActorState::None);
		CurrentRoofActor = nullptr;
		PlayerController->AddSuccessMessage(FText::FromString("Roof Deselected Successfully"), 1.5f);
	}
}
