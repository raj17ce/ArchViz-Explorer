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
		if ((CurrentDoorActor->GetState() == EBuildingActorState::Preview) || (CurrentDoorActor->GetState() == EBuildingActorState::Moving)) {	
			CurrentDoorActor->SetState(EBuildingActorState::None);
			CurrentDoorActor->Destroy();
		}
		else {
			CurrentDoorActor->SetState(EBuildingActorState::None);
		}
		CurrentDoorActor = nullptr;
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

			//Delete-Key
			auto* DeleteKeyPressAction = NewObject<UInputAction>(this);
			DeleteKeyPressAction->ValueType = EInputActionValueType::Boolean;

			MappingContext->MapKey(DeleteKeyPressAction, EKeys::Delete);
			EIC->BindAction(DeleteKeyPressAction, ETriggerEvent::Completed, this, &UDoorSubMode::HandleDoorDeleteButtonClick);
		}
	}
}

void UDoorSubMode::HandleLeftMouseClick() {
	if (IsValid(DoorActorClass)) {

		switch (SubModeState) {
		case EBuildingSubModeState::Free:
			HandleFreeState();
			break;
		case EBuildingSubModeState::OldObject:
			//Handling Logic for OldObject and NewObject is Same
			[[fallthrough]];
		case EBuildingSubModeState::NewObject:
			HandleNewObjectState();
			break;
		}
	}
}

void UDoorSubMode::HandleRKeyPress() {
	if (IsValid(CurrentDoorActor)) {
		CurrentDoorActor->RotateActor(180.0);
	}
}

void UDoorSubMode::HandleMKeyPress() {
	if (IsValid(CurrentDoorActor)) {
		if (auto* WallActor = Cast<AWallActor>(CurrentDoorActor->GetAttachParentActor())) {
			WallActor->DetachDoorComponent(CurrentDoorActor);
		}

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

void UDoorSubMode::HandleFreeState() {
	FHitResult HitResult = GetHitResult();

	if (IsValid(CurrentDoorActor)) {
		CurrentDoorActor->SetState(EBuildingActorState::None);
		CurrentDoorActor = nullptr;
	}

	if (HitResult.GetActor() && HitResult.GetActor()->IsA(ADoorActor::StaticClass())) {
		CurrentDoorActor = Cast<ADoorActor>(HitResult.GetActor());
		CurrentDoorActor->SetState(EBuildingActorState::Selected);
		//To-Do Display Widget
	}
	else {
		if (DoorActorClass) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			CurrentDoorActor = GetWorld()->SpawnActor<ADoorActor>(DoorActorClass, SpawnParams);
			BindWidgetDelegates();
			CurrentDoorActor->SetState(EBuildingActorState::Preview);
			SubModeState = EBuildingSubModeState::NewObject;
			//To-Do Preview Material
		}
	}
}

void UDoorSubMode::HandleNewObjectState() {
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
}

void UDoorSubMode::BindWidgetDelegates() {
	if (IsValid(CurrentDoorActor) && IsValid(CurrentDoorActor->PropertyPanelWidget)) {
		CurrentDoorActor->PropertyPanelWidget->DoorNewButton->OnClicked.AddDynamic(this, &UDoorSubMode::HandleDoorNewButtonClick);
		CurrentDoorActor->PropertyPanelWidget->DoorDeleteButton->OnClicked.AddDynamic(this, &UDoorSubMode::HandleDoorDeleteButtonClick);
		CurrentDoorActor->PropertyPanelWidget->DoorCloseButton->OnClicked.AddDynamic(this, &UDoorSubMode::HandleDoorCloseButtonClick);
	}
}

void UDoorSubMode::HandleDoorNewButtonClick() {
	if (IsValid(CurrentDoorActor)) {
		CurrentDoorActor->SetState(EBuildingActorState::None);
		CurrentDoorActor = nullptr;

		if (IsValid(DoorActorClass)) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			CurrentDoorActor = GetWorld()->SpawnActor<ADoorActor>(DoorActorClass, SpawnParams);
			BindWidgetDelegates();
			CurrentDoorActor->SetState(EBuildingActorState::Preview);
			SubModeState = EBuildingSubModeState::NewObject;
		}
	}
}

void UDoorSubMode::HandleDoorDeleteButtonClick() {
	if (IsValid(CurrentDoorActor)) {
		if (auto* WallActor = Cast<AWallActor>(CurrentDoorActor->GetAttachParentActor())) {
			WallActor->DetachDoorComponent(CurrentDoorActor);
		}
		CurrentDoorActor->SetState(EBuildingActorState::None);
		CurrentDoorActor->Destroy();
		CurrentDoorActor = nullptr;
	}
}

void UDoorSubMode::HandleDoorCloseButtonClick() {
	if (IsValid(CurrentDoorActor)) {
		CurrentDoorActor->SetState(EBuildingActorState::None);
		CurrentDoorActor = nullptr;
	}
}
