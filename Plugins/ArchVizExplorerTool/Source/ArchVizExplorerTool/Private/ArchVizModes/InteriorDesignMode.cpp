// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/InteriorDesignMode.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"

void UInteriorDesignMode::Setup() {
	CurrentInteriorActor = nullptr;
	InteriorModeState = EInteriorModeState::Free;
	if (IsValid(WidgetClass)) {
		Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass, "Interior Widget");
	}
}

void UInteriorDesignMode::EnterMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->AddMappingContext(MappingContext, 0);
			ShowWidget();
			Setup();
		}
	}
}

void UInteriorDesignMode::ExitMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->RemoveMappingContext(MappingContext);
			HideWidget();
		}
	}
}

void UInteriorDesignMode::SetPlayerController(APlayerController* Controller) {
	PlayerController = Controller;
}

void UInteriorDesignMode::SetupInputComponent() {
	if (auto* EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {

		MappingContext = NewObject<UInputMappingContext>();

		//Left-Click
		auto* LeftMouseClickAction = NewObject<UInputAction>();
		LeftMouseClickAction->ValueType = EInputActionValueType::Boolean;

		MappingContext->MapKey(LeftMouseClickAction, EKeys::LeftMouseButton);
		EIC->BindAction(LeftMouseClickAction, ETriggerEvent::Completed, this, &UInteriorDesignMode::HandleLeftMouseClick);
	}
}

void UInteriorDesignMode::HandleInteriorAssetSelect(EInteriorAssetType AssetType, UStaticMesh* StaticMesh) {
	if (InteriorActorClass) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		CurrentInteriorActor = GetWorld()->SpawnActor<AInteriorActor>(InteriorActorClass, SpawnParams);
		CurrentInteriorActor->SetInteriorAssetType(AssetType);
		CurrentInteriorActor->SetState(EInteriorActorState::Preview);
		InteriorModeState = EInteriorModeState::NewObject;
		// Todo: Set STatic Mesh
		//	// Property Widget
		//	// BindWidgetDelegates();
	}
}

void UInteriorDesignMode::HandleLeftMouseClick() {
	switch (InteriorModeState) {
	case EInteriorModeState::Free:	
		HandleFreeState();
		break;
	case EInteriorModeState::OldObject:
		HandleOldObjectState();
		break;
	case EInteriorModeState::NewObject:
		HandleNewObjectState();
		break;
	}
}

void UInteriorDesignMode::HandleFreeState() {
	FHitResult HitResult = GetHitResult();

	if (IsValid(CurrentInteriorActor)) {
		CurrentInteriorActor->SetState(EInteriorActorState::None);
		CurrentInteriorActor = nullptr;
	}

	if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(AInteriorActor::StaticClass())) {
		CurrentInteriorActor = Cast<AInteriorActor>(HitResult.GetActor());
		CurrentInteriorActor->SetState(EInteriorActorState::Selected);
	}
	else {
		//if (IsValid(InteriorActorClass)) {
		//	FActorSpawnParameters SpawnParams;
		//	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//	CurrentInteriorActor = GetWorld()->SpawnActor<AInteriorActor>(InteriorActorClass, SpawnParams);
		//	// Property Widget
		//	// BindWidgetDelegates();
		//	CurrentInteriorActor->SetState(EInteriorActorState::Preview);
		//	InteriorModeState = EInteriorModeState::NewObject;
		//	//To-Do Preview Material
		//}
	}
}

void UInteriorDesignMode::HandleOldObjectState() {
	
}

void UInteriorDesignMode::HandleNewObjectState() {
	
}