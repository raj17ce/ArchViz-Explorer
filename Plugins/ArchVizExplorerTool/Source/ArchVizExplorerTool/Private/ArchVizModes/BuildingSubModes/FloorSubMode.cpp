// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/BuildingSubModes/FloorSubMode.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ArchVizUtility.h"
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

void UFloorSubMode::SelectActor(AFloorActor* FloorActor) {
	if (IsValid(CurrentFloorActor)) {
		CurrentFloorActor->SetState(EBuildingActorState::None);
		CurrentFloorActor = nullptr;
	}

	CurrentFloorActor = FloorActor;

	if (IsValid(CurrentFloorActor)) {
		CurrentFloorActor->SetState(EBuildingActorState::Selected);
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

	if (IsValid(CurrentFloorActor)) {
		CurrentFloorActor->SetState(EBuildingActorState::None);
		CurrentFloorActor = nullptr;
	}

	if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(AFloorActor::StaticClass())) {
		CurrentFloorActor = Cast<AFloorActor>(HitResult.GetActor());
		CurrentFloorActor->SetState(EBuildingActorState::Selected);
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
			BindWidgetDelegates();
			CurrentFloorActor->GenerateFloor(FVector{ 100.0,100.0, 2.0 }, FVector{ 50.0,50.0, 1.0 });
			CurrentFloorActor->SetState(EBuildingActorState::Preview);
			SubModeState = EBuildingSubModeState::NewObject;
			//To-Do Preview Material
		}
	}
}

void UFloorSubMode::HandleOldObjectState() {
	if (IsValid(CurrentFloorActor)) {
		SubModeState = EBuildingSubModeState::Free;
		CurrentFloorActor->SetState(EBuildingActorState::Selected);
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
		}
		else {
			bNewFloorStart = false;
			CurrentFloorActor->SetEndPoint(HitResult.Location);
			CurrentFloorActor->UpdateSpinBoxValue();
			CurrentFloorActor->SetState(EBuildingActorState::Selected);
			SubModeState = EBuildingSubModeState::Free;
		}
	}
}

void UFloorSubMode::BindWidgetDelegates() {
	if (IsValid(CurrentFloorActor) && IsValid(CurrentFloorActor->PropertyPanelWidget)) {
		CurrentFloorActor->PropertyPanelWidget->FloorNewButton->OnClicked.AddDynamic(this, &UFloorSubMode::HandleFloorNewButtonClick);
		CurrentFloorActor->PropertyPanelWidget->FloorDeleteButton->OnClicked.AddDynamic(this, &UFloorSubMode::HandleFloorDeleteButtonClick);
		CurrentFloorActor->PropertyPanelWidget->FloorCloseButton->OnClicked.AddDynamic(this, &UFloorSubMode::HandleFloorCloseButtonClick);
		CurrentFloorActor->PropertyPanelWidget->FloorLengthSpinbox->OnValueChanged.AddDynamic(this, &UFloorSubMode::HandleFloorSpinBoxValueChange);
		CurrentFloorActor->PropertyPanelWidget->FloorWidthSpinbox->OnValueChanged.AddDynamic(this, &UFloorSubMode::HandleFloorSpinBoxValueChange);
		CurrentFloorActor->PropertyPanelWidget->FloorHeightSpinbox->OnValueChanged.AddDynamic(this, &UFloorSubMode::HandleFloorSpinBoxValueChange);
	}
}

void UFloorSubMode::HandleFloorSpinBoxValueChange(float InLength) {
	if (IsValid(CurrentFloorActor) && IsValid(CurrentFloorActor->PropertyPanelWidget)) {
		float Length = CurrentFloorActor->PropertyPanelWidget->FloorLengthSpinbox->GetValue();
		float Width = CurrentFloorActor->PropertyPanelWidget->FloorWidthSpinbox->GetValue();
		float Height = CurrentFloorActor->PropertyPanelWidget->FloorHeightSpinbox->GetValue();;

		double EdgeOffset{ 10.0 };

		FVector Dimensions{ Length + (2 * EdgeOffset), Width + (2 * EdgeOffset), Height };
		FVector Offset{ Length / 2 , Width / 2, Height / 2 };

		double XDistance = CurrentFloorActor->EndPoint.X - CurrentFloorActor->StartPoint.X;
		double YDistance = CurrentFloorActor->EndPoint.Y - CurrentFloorActor->StartPoint.Y;

		if (XDistance >= 0.0 && YDistance < 0.0) {
			Offset.Z *= -1.0;
		}
		else if (XDistance < 0.0 && YDistance >= 0.0) {
			Offset.Z *= -1.0;
		}

		CurrentFloorActor->GenerateFloor(Dimensions, Offset);
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
			BindWidgetDelegates();
			CurrentFloorActor->GenerateFloor(FVector{ 100.0,100.0, 2.0 }, FVector{ 50.0,50.0, 1.0 });
			CurrentFloorActor->SetState(EBuildingActorState::Preview);
			SubModeState = EBuildingSubModeState::NewObject;
		}
	}
}

void UFloorSubMode::HandleFloorDeleteButtonClick() {
	if (IsValid(CurrentFloorActor)) {
		CurrentFloorActor->SetState(EBuildingActorState::None);
		CurrentFloorActor->Destroy();
		CurrentFloorActor = nullptr;
	}
}

void UFloorSubMode::HandleFloorCloseButtonClick() {
	if (IsValid(CurrentFloorActor)) {
		CurrentFloorActor->SetState(EBuildingActorState::None);
		CurrentFloorActor = nullptr;
	}
}
