// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingSubModes/FloorSubMode.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ArchVizUtility.h"
#include "ArchVizController.h"
#include "ArchVizActors/BuildingActors/WallActor.h"
#include "ArchVizActors/BuildingActors/DoorActor.h"
#include "ArchVizActors/BuildingActors/RoofActor.h"

void UFloorSubMode::Setup() {
	bNewFloorStart = false;
	CurrentFloorActor = nullptr;
	SubModeState = EBuildingSubModeState::Free;
}

void UFloorSubMode::Cleanup() {
	if (IsValid(CurrentFloorActor)) {
		if ((CurrentFloorActor->GetState() == EBuildingActorState::Preview) || (CurrentFloorActor->GetState() == EBuildingActorState::Generating)) {
			CurrentFloorActor->SetState(EBuildingActorState::None);
			CurrentFloorActor->Destroy();
		}
		else {
			CurrentFloorActor->SetState(EBuildingActorState::None);
		}
		CurrentFloorActor = nullptr;
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

			//Delete-Key
			auto* DeleteKeyPressAction = NewObject<UInputAction>(this);
			DeleteKeyPressAction->ValueType = EInputActionValueType::Boolean;

			MappingContext->MapKey(DeleteKeyPressAction, EKeys::Delete);
			EIC->BindAction(DeleteKeyPressAction, ETriggerEvent::Completed, this, &UFloorSubMode::HandleFloorDeleteButtonClick);
		}
	}
}

void UFloorSubMode::BindPropertyDelegatesToActor(ABuildingActor* Actor) {
	if (auto* FloorActor = Cast<AFloorActor>(Actor)) {
		if (IsValid(FloorActor) && IsValid(FloorActor->PropertyPanelWidget)) {
			FloorActor->PropertyPanelWidget->FloorNewButton->OnClicked.AddDynamic(this, &UFloorSubMode::HandleFloorNewButtonClick);
			FloorActor->PropertyPanelWidget->FloorDeleteButton->OnClicked.AddDynamic(this, &UFloorSubMode::HandleFloorDeleteButtonClick);
			FloorActor->PropertyPanelWidget->FloorCloseButton->OnClicked.AddDynamic(this, &UFloorSubMode::HandleFloorCloseButtonClick);
			FloorActor->PropertyPanelWidget->FloorLengthSpinbox->OnValueChanged.AddDynamic(this, &UFloorSubMode::HandleFloorSpinBoxValueChange);
			FloorActor->PropertyPanelWidget->FloorWidthSpinbox->OnValueChanged.AddDynamic(this, &UFloorSubMode::HandleFloorSpinBoxValueChange);
			FloorActor->PropertyPanelWidget->FloorHeightSpinbox->OnValueChanged.AddDynamic(this, &UFloorSubMode::HandleFloorSpinBoxValueChange);
		}
	}
}

void UFloorSubMode::SelectActor(AFloorActor* FloorActor) {
	if (IsValid(CurrentFloorActor)) {
		CurrentFloorActor->SetState(EBuildingActorState::None);
		CurrentFloorActor = nullptr;
	}

	CurrentFloorActor = FloorActor;

	if (IsValid(CurrentFloorActor)) {
		CurrentFloorActor->SetState(EBuildingActorState::Selected);
		PlayerController->AddSuccessMessage(FText::FromString("Floor Selected Successfully"), 1.5f);
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
		PlayerController->AddSuccessMessage(FText::FromString("Floor Rotated Successfully"), 1.5f);
	}
}

void UFloorSubMode::HandleMKeyPress() {
	if (IsValid(CurrentFloorActor) && CurrentFloorActor->GetState() == EBuildingActorState::Selected) {
		CurrentFloorActor->SetState(EBuildingActorState::Moving);
		SubModeState = EBuildingSubModeState::OldObject;
	}
}

void UFloorSubMode::HandleFreeState() {
	FHitResult HitResult = GetHitResult();

	if (IsValid(CurrentFloorActor)) {
		CurrentFloorActor->SetState(EBuildingActorState::None);
		CurrentFloorActor = nullptr;
	}

	if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(AFloorActor::StaticClass())) {
		CurrentFloorActor = Cast<AFloorActor>(HitResult.GetActor());
		CurrentFloorActor->SetState(EBuildingActorState::Selected);
		PlayerController->AddSuccessMessage(FText::FromString("Floor Selected Successfully"), 1.5f);
	}
	else if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(AWallActor::StaticClass())) {
		OnOtherBuildingActorSelected.ExecuteIfBound(EBuildingSubMode::WallConstruction, HitResult.GetActor());
	}
	else if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(ADoorActor::StaticClass())) {
		OnOtherBuildingActorSelected.ExecuteIfBound(EBuildingSubMode::DoorConstruction, HitResult.GetActor());
	}
	else if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(ARoofActor::StaticClass())) {
		OnOtherBuildingActorSelected.ExecuteIfBound(EBuildingSubMode::RoofConstruction, HitResult.GetActor());
	}
	else {
		if (IsValid(FloorActorClass)) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			CurrentFloorActor = GetWorld()->SpawnActor<AFloorActor>(FloorActorClass, SpawnParams);
			BindPropertyDelegatesToActor(CurrentFloorActor);
			CurrentFloorActor->GenerateFloor();
			CurrentFloorActor->SetState(EBuildingActorState::Preview);
			SubModeState = EBuildingSubModeState::NewObject;
			PlayerController->AddSuccessMessage(FText::FromString("Floor Preview Started"), 1.5f);
		}
	}
}

void UFloorSubMode::HandleOldObjectState() {
	if (IsValid(CurrentFloorActor)) {
		SubModeState = EBuildingSubModeState::Free;
		CurrentFloorActor->SetState(EBuildingActorState::Selected);
		PlayerController->AddSuccessMessage(FText::FromString("Floor Moved Successfully"), 1.5f);
	}
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
			PlayerController->AddSuccessMessage(FText::FromString("Floor Generation Started"), 1.5f);
		}
		else {
			bNewFloorStart = false;
			CurrentFloorActor->SetEndPoint(HitResult.Location);
			CurrentFloorActor->UpdateSpinBoxValue();
			CurrentFloorActor->SetState(EBuildingActorState::Selected);
			SubModeState = EBuildingSubModeState::Free;
			PlayerController->AddSuccessMessage(FText::FromString("Floor Generated Successfully"), 1.5f);
		}
	}
}

void UFloorSubMode::HandleFloorSpinBoxValueChange(float InLength) {
	if (IsValid(CurrentFloorActor) && IsValid(CurrentFloorActor->PropertyPanelWidget)) {
		float Length = CurrentFloorActor->PropertyPanelWidget->FloorLengthSpinbox->GetValue();
		float Width = CurrentFloorActor->PropertyPanelWidget->FloorWidthSpinbox->GetValue();
		float Height = CurrentFloorActor->PropertyPanelWidget->FloorHeightSpinbox->GetValue();;

		double EdgeOffset{ 10.0 };

		CurrentFloorActor->SetDimensions(FVector{ Length + (2 * EdgeOffset), Width + (2 * EdgeOffset), Height });
		CurrentFloorActor->SetOffset(FVector{ Length / 2 , Width / 2, Height / 2 });
		
		CurrentFloorActor->GenerateFloor();
		PlayerController->AddSuccessMessage(FText::FromString("Floor Updated Successfully"), 1.5f);
	}
}

void UFloorSubMode::HandleFloorNewButtonClick() {
	if (IsValid(CurrentFloorActor)) {
		CurrentFloorActor->SetState(EBuildingActorState::None);
		CurrentFloorActor = nullptr;

		if (IsValid(FloorActorClass)) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			CurrentFloorActor = GetWorld()->SpawnActor<AFloorActor>(FloorActorClass, SpawnParams);
			BindPropertyDelegatesToActor(CurrentFloorActor);
			CurrentFloorActor->GenerateFloor();
			CurrentFloorActor->SetState(EBuildingActorState::Preview);
			SubModeState = EBuildingSubModeState::NewObject;
			PlayerController->AddSuccessMessage(FText::FromString("Floor Preview Started"), 1.5f);
		}
	}
}

void UFloorSubMode::HandleFloorDeleteButtonClick() {
	if (IsValid(CurrentFloorActor) && CurrentFloorActor->GetState() == EBuildingActorState::Selected) {
		CurrentFloorActor->SetState(EBuildingActorState::None);
		CurrentFloorActor->Destroy();
		CurrentFloorActor = nullptr;
		PlayerController->AddSuccessMessage(FText::FromString("Floor Deleted Successfully"), 1.5f);
	}
}

void UFloorSubMode::HandleFloorCloseButtonClick() {
	if (IsValid(CurrentFloorActor)) {
		CurrentFloorActor->SetState(EBuildingActorState::None);
		CurrentFloorActor = nullptr;
		PlayerController->AddSuccessMessage(FText::FromString("Floor Deselected Successfully"), 1.5f);
	}
}
