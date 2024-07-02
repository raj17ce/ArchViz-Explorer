// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/RoadConstructionMode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void URoadConstructionMode::Setup() {
	CurrentRoadActor = nullptr;
	RoadModeState = ERoadModeState::Free;

	if (IsValid(WidgetClass) && !IsValid(Widget)) {
		Widget = CreateWidget<URoadConstructionWidget>(GetWorld(), WidgetClass, "Road Widget");

		if (auto* RoadWidget = Cast<URoadConstructionWidget>(Widget)) {
			RoadWidget->NewRoadButton->OnClicked.AddDynamic(this, &URoadConstructionMode::HandleRoadNewButtonClick);
			RoadWidget->SaveRoadButton->OnClicked.AddDynamic(this, &URoadConstructionMode::HandleSaveRoadButtonClick);
			RoadWidget->UndoRoadButton->OnClicked.AddDynamic(this, &URoadConstructionMode::HandleUndoRoadButtonClick);
		}
	}
}

void URoadConstructionMode::Cleanup() {
	if (IsValid(CurrentRoadActor)) {
		CurrentRoadActor->SetState(ERoadActorState::None);
		CurrentRoadActor = nullptr;
		RoadModeState = ERoadModeState::Free;
	}
}

void URoadConstructionMode::EnterMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->AddMappingContext(MappingContext, 0);
			ShowWidget();
			Setup();
		}
	}
}

void URoadConstructionMode::ExitMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->RemoveMappingContext(MappingContext);
			HideWidget();
			Cleanup();
		}
	}
}

void URoadConstructionMode::SetupInputComponent() {
	if (auto* EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {

		MappingContext = NewObject<UInputMappingContext>();

		//Left-Click
		auto* LeftMouseClickAction = NewObject<UInputAction>();
		LeftMouseClickAction->ValueType = EInputActionValueType::Boolean;

		MappingContext->MapKey(LeftMouseClickAction, EKeys::LeftMouseButton);
		EIC->BindAction(LeftMouseClickAction, ETriggerEvent::Completed, this, &URoadConstructionMode::HandleLeftMouseClick);

		//Delete-Key
		auto* DeleteKeyPressAction = NewObject<UInputAction>(this);
		DeleteKeyPressAction->ValueType = EInputActionValueType::Boolean;

		MappingContext->MapKey(DeleteKeyPressAction, EKeys::Delete);
		EIC->BindAction(DeleteKeyPressAction, ETriggerEvent::Completed, this, &URoadConstructionMode::HandleRoadDeleteButtonClick);
	}
}

void URoadConstructionMode::HandleLeftMouseClick() {
	switch (RoadModeState) {
	case ERoadModeState::Free:
		HandleFreeState();
		break;
	case ERoadModeState::NewObject:
		HandleNewObjectState();
		break;
	}
}

void URoadConstructionMode::HandleFreeState() {
	FHitResult HitResult = GetHitResult();

	if (IsValid(CurrentRoadActor)) {
		CurrentRoadActor->SetState(ERoadActorState::None);
		CurrentRoadActor = nullptr;
	}

	if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(ARoadActor::StaticClass())) {
		CurrentRoadActor = Cast<ARoadActor>(HitResult.GetActor());
		CurrentRoadActor->SetState(ERoadActorState::Selected);
		RoadModeState = ERoadModeState::Free;
	}
}

void URoadConstructionMode::HandleNewObjectState() {
	if (IsValid(CurrentRoadActor)) {
		FHitResult HitResult{};

		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(CurrentRoadActor);

		HitResult = CurrentRoadActor->GetHitResult(IgnoredActors);
		CurrentRoadActor->AddSplinePoint(HitResult.Location);
	}
}

void URoadConstructionMode::BindPropertyWidgetDelegates() {
	if (IsValid(CurrentRoadActor)) {
		CurrentRoadActor->OnRoadActorStateChanged.BindUObject(this, &URoadConstructionMode::HandleRoadActorStateChange);
		if (IsValid(CurrentRoadActor->PropertyPanelWidget)) {
			CurrentRoadActor->PropertyPanelWidget->RoadNewButton->OnClicked.AddDynamic(this, &URoadConstructionMode::HandleRoadNewButtonClick);
			CurrentRoadActor->PropertyPanelWidget->RoadDeleteButton->OnClicked.AddDynamic(this, &URoadConstructionMode::HandleRoadDeleteButtonClick);
			CurrentRoadActor->PropertyPanelWidget->RoadCloseButton->OnClicked.AddDynamic(this, &URoadConstructionMode::HandleRoadCloseButtonClick);
			CurrentRoadActor->PropertyPanelWidget->RoadWidthSpinbox->OnValueChanged.AddDynamic(this, &URoadConstructionMode::HandleRoadWidthChange);
			CurrentRoadActor->PropertyPanelWidget->RoadTypeComboBox->OnSelectionChanged.AddDynamic(this, &URoadConstructionMode::HandleRoadTypeChange);
		}
	}
}

void URoadConstructionMode::HandleSaveRoadButtonClick() {
	if (IsValid(CurrentRoadActor)) {
		CurrentRoadActor->UpdatePropertyPanelValues();
		CurrentRoadActor->SetState(ERoadActorState::Selected);
		RoadModeState = ERoadModeState::Free;
	}
}

void URoadConstructionMode::HandleUndoRoadButtonClick() {
	if (IsValid(CurrentRoadActor) && CurrentRoadActor->GetState() == ERoadActorState::Generating) {
		CurrentRoadActor->RemoveLastSplinePoint();
	}
}

void URoadConstructionMode::HandleRoadWidthChange(float InWidth) {
	if (IsValid(CurrentRoadActor)) {
		CurrentRoadActor->Width = InWidth;
		CurrentRoadActor->UpdateRoad();
	}	
}

void URoadConstructionMode::HandleRoadTypeChange(FString SelectedType, ESelectInfo::Type SelectionType) {
	if (SelectedType == TEXT("Curved")) {
		CurrentRoadActor->SetRoadType(ERoadType::Curved);
	}
	else if (SelectedType == TEXT("Sharp")) {
		CurrentRoadActor->SetRoadType(ERoadType::Sharp);
	}
}

void URoadConstructionMode::HandleRoadNewButtonClick() {
	if (IsValid(CurrentRoadActor)) {
		CurrentRoadActor->SetState(ERoadActorState::None);
		CurrentRoadActor = nullptr;
	}

	if (IsValid(RoadActorClass) && !IsValid(CurrentRoadActor)) {
		CurrentRoadActor = GetWorld()->SpawnActor<ARoadActor>(RoadActorClass, FTransform{});
		BindPropertyWidgetDelegates();
		CurrentRoadActor->SetState(ERoadActorState::Generating);
		RoadModeState = ERoadModeState::NewObject;
	}
}

void URoadConstructionMode::HandleRoadDeleteButtonClick() {
	if (IsValid(CurrentRoadActor) && CurrentRoadActor->GetState() == ERoadActorState::Selected) {
		CurrentRoadActor->SetState(ERoadActorState::None);
		CurrentRoadActor->SplineComponent->ClearSplinePoints();
		CurrentRoadActor->Destroy();
		CurrentRoadActor = nullptr;
		RoadModeState = ERoadModeState::Free;
	}
}

void URoadConstructionMode::HandleRoadCloseButtonClick() {
	if (IsValid(CurrentRoadActor)) {
		CurrentRoadActor->SetState(ERoadActorState::None);
		CurrentRoadActor = nullptr;
	}
}

void URoadConstructionMode::HandleRoadActorStateChange(ERoadActorState RoadActorState) {
	if (auto* RoadWidget = Cast<URoadConstructionWidget>(Widget)) {
		if (RoadActorState == ERoadActorState::Generating) {
			RoadWidget->ShowUndoButton();
		}
		else {
			RoadWidget->HideUndoButton();
		}
	}
}
