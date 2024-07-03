// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizModes/InteriorDesignMode.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ArchVizController.h"
#include "ArchVizActors/BuildingActors/FloorActor.h"
#include "ArchVizActors/BuildingActors/WallActor.h"
#include "ArchVizActors/BuildingActors/RoofActor.h"
#include "Widgets/InteriorDesignWidget.h"
#include "ArchVizActors/InteriorActor.h"

void UInteriorDesignMode::Setup() {
	CurrentInteriorActor = nullptr;
	InteriorModeState = EInteriorModeState::Free;
	if (IsValid(WidgetClass) && !IsValid(Widget)) {
		Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass, "Interior Widget");
		if (auto* InteriorWidget = Cast<UInteriorDesignWidget>(Widget)) {
			if (IsValid(InteriorWidget->InteriorScrollBox)) {
				InteriorWidget->InteriorScrollBox->OnItemSelected.BindUObject(this, &UInteriorDesignMode::HandleInteriorAssetSelect);
				InteriorWidget->OnInteriorTypeChange.BindLambda([this]() {
					if (IsValid(CurrentInteriorActor)) {
						if ((CurrentInteriorActor->GetState() == EInteriorActorState::Preview)) {
							CurrentInteriorActor->SetState(EInteriorActorState::None);
							CurrentInteriorActor->Destroy();
						}
						CurrentInteriorActor->SetState(EInteriorActorState::None);
						CurrentInteriorActor = nullptr;
					}
				});
			}
		}
	}
}

void UInteriorDesignMode::EnterMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->AddMappingContext(MappingContext, 0);
			ShowWidget();
			Setup();

			if (auto* InteriorWidget = Cast<UInteriorDesignWidget>(Widget)) {
				InteriorWidget->ShowScrollBox();
			}
		}
	}
}

void UInteriorDesignMode::ExitMode() {
	if (IsValid(PlayerController)) {
		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			LocalPlayerSubsystem->RemoveMappingContext(MappingContext);
			HideWidget();
			Cleanup();
		}
	}
}

void UInteriorDesignMode::SetupInputComponent() {
	if (auto* EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {

		MappingContext = NewObject<UInputMappingContext>();

		//Left-Click
		auto* LeftMouseClickAction = NewObject<UInputAction>();
		LeftMouseClickAction->ValueType = EInputActionValueType::Boolean;

		MappingContext->MapKey(LeftMouseClickAction, EKeys::LeftMouseButton);
		EIC->BindAction(LeftMouseClickAction, ETriggerEvent::Completed, this, &UInteriorDesignMode::HandleLeftMouseClick);

		//R-Key
		auto* RKeyPressAction = NewObject<UInputAction>(this);
		RKeyPressAction->ValueType = EInputActionValueType::Boolean;

		MappingContext->MapKey(RKeyPressAction, EKeys::R);
		EIC->BindAction(RKeyPressAction, ETriggerEvent::Completed, this, &UInteriorDesignMode::HandleRKeyPress);

		//M-Key
		auto* MKeyPressAction = NewObject<UInputAction>(this);
		MKeyPressAction->ValueType = EInputActionValueType::Boolean;

		MappingContext->MapKey(MKeyPressAction, EKeys::M);
		EIC->BindAction(MKeyPressAction, ETriggerEvent::Completed, this, &UInteriorDesignMode::HandleMKeyPress);

		//Delete-Key
		auto* DeleteKeyPressAction = NewObject<UInputAction>(this);
		DeleteKeyPressAction->ValueType = EInputActionValueType::Boolean;

		MappingContext->MapKey(DeleteKeyPressAction, EKeys::Delete);
		EIC->BindAction(DeleteKeyPressAction, ETriggerEvent::Completed, this, &UInteriorDesignMode::HandleInteriorDeleteButtonClick);
	}
}

void UInteriorDesignMode::BindPropertyDelegatesToActor(AArchVizActor* Actor) {
	if (auto InteriorActor = Cast<AInteriorActor>(Actor)) {
		if(IsValid(InteriorActor->PropertyPanelWidget)) {
			InteriorActor->PropertyPanelWidget->InteriorNewButton->OnClicked.AddDynamic(this, &UInteriorDesignMode::HandleInteriorNewButtonClick);
			InteriorActor->PropertyPanelWidget->InteriorDeleteButton->OnClicked.AddDynamic(this, &UInteriorDesignMode::HandleInteriorDeleteButtonClick);
			InteriorActor->PropertyPanelWidget->InteriorCloseButton->OnClicked.AddDynamic(this, &UInteriorDesignMode::HandleInteriorCloseButtonClick);
		}
	}
}

void UInteriorDesignMode::Cleanup() {
	if (IsValid(CurrentInteriorActor)) {
		if ((CurrentInteriorActor->GetState() == EInteriorActorState::Preview)) {
			CurrentInteriorActor->SetState(EInteriorActorState::None);
			CurrentInteriorActor->Destroy();
		}
		else {
			CurrentInteriorActor->SetState(EInteriorActorState::None);
		}
		CurrentInteriorActor = nullptr;
	}
}

void UInteriorDesignMode::HandleInteriorAssetSelect(FInteriorAssetData AssetData) {
	if (InteriorActorClass) {

		if (IsValid(CurrentInteriorActor)) {
			if ((CurrentInteriorActor->GetState() == EInteriorActorState::Preview)) {
				CurrentInteriorActor->SetState(EInteriorActorState::None);
				CurrentInteriorActor->Destroy();
			}
			CurrentInteriorActor->SetState(EInteriorActorState::None);
			CurrentInteriorActor = nullptr;
		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		CurrentInteriorActor = GetWorld()->SpawnActor<AInteriorActor>(InteriorActorClass, SpawnParams);
		BindPropertyDelegatesToActor(CurrentInteriorActor);
		CurrentInteriorActor->SetActorAssetData(AssetData);
		CurrentInteriorActor->SetState(EInteriorActorState::Preview);
		InteriorModeState = EInteriorModeState::NewObject;
		PlayerController->AddSuccessMessage(FText::FromString("Interior Preview Started"), 1.5f);
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

void UInteriorDesignMode::HandleRKeyPress() {
	if (IsValid(CurrentInteriorActor)) {
		CurrentInteriorActor->RotateActor(90.0);
		PlayerController->AddSuccessMessage(FText::FromString("Interior Rotated Successfully"), 1.5f);
	}
}

void UInteriorDesignMode::HandleMKeyPress() {
	if (IsValid(CurrentInteriorActor) && CurrentInteriorActor->GetState() == EInteriorActorState::Selected) {
		CurrentInteriorActor->SetState(EInteriorActorState::Moving);
		InteriorModeState = EInteriorModeState::OldObject;
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
		PlayerController->AddSuccessMessage(FText::FromString("Interior Selected Successfully"), 1.5f);

		if (auto* InteriorWidget = Cast<UInteriorDesignWidget>(Widget)) {
			InteriorWidget->HideScrollBox();
		}
	}
	else {
		PlayerController->AddErrorMessage(FText::FromString("Please Select an Interior Object"), 1.5f);
	}
}

void UInteriorDesignMode::HandleOldObjectState() {
	if (IsValid(CurrentInteriorActor)) {
		InteriorModeState = EInteriorModeState::Free;
		CurrentInteriorActor->SetState(EInteriorActorState::Selected);
		PlayerController->AddSuccessMessage(FText::FromString("Interior Moved Successfully"), 1.5f);
	}
}

void UInteriorDesignMode::HandleNewObjectState() {
	if (IsValid(CurrentInteriorActor)) {

		FHitResult HitResult = GetHitResult(TArray<AActor*> {CurrentInteriorActor});
		
		if (IsValid(HitResult.GetActor())) {
			switch (CurrentInteriorActor->AssetData.InteriorAssetType) {
			case EInteriorAssetType::FloorPlaceable:
				if (HitResult.GetActor()->IsA(AFloorActor::StaticClass())) {
					CurrentInteriorActor->AttachToActor(HitResult.GetActor(), FAttachmentTransformRules::KeepWorldTransform);
					CurrentInteriorActor->SetActorLocation(HitResult.Location);
					CurrentInteriorActor->SetState(EInteriorActorState::Selected);
					InteriorModeState = EInteriorModeState::Free;
					if (auto* InteriorWidget = Cast<UInteriorDesignWidget>(Widget)) {
						InteriorWidget->HideScrollBox();
					}
				}
				else {
					PlayerController->AddErrorMessage(FText::FromString("This Interior can only be placed on a Floor"), 1.5f);
				}
				break;
			case EInteriorAssetType::WallPlaceable:
				if (HitResult.GetActor()->IsA(AWallActor::StaticClass())) {
					CurrentInteriorActor->AttachToActor(HitResult.GetActor(), FAttachmentTransformRules::KeepWorldTransform);
					CurrentInteriorActor->SetActorLocation(HitResult.Location);
					FRotator ActorRotation = HitResult.GetActor()->GetActorRotation();
					CurrentInteriorActor->AdjustWallInteriorRotation(ActorRotation);
					CurrentInteriorActor->SetActorRotation(ActorRotation);
					CurrentInteriorActor->SetState(EInteriorActorState::Selected);
					InteriorModeState = EInteriorModeState::Free;
					if (auto* InteriorWidget = Cast<UInteriorDesignWidget>(Widget)) {
						InteriorWidget->HideScrollBox();
					}
				}
				else {
					PlayerController->AddErrorMessage(FText::FromString("This Interior can only be placed on a Wall"), 1.5f);
				}
				break;
			case EInteriorAssetType::RoofPlaceable:
				if (HitResult.GetActor()->IsA(ARoofActor::StaticClass())) {
					CurrentInteriorActor->AttachToActor(HitResult.GetActor(), FAttachmentTransformRules::KeepWorldTransform);
					CurrentInteriorActor->SetActorLocation(HitResult.Location);
					CurrentInteriorActor->SetState(EInteriorActorState::Selected);
					InteriorModeState = EInteriorModeState::Free;
					if (auto* InteriorWidget = Cast<UInteriorDesignWidget>(Widget)) {
						InteriorWidget->HideScrollBox();
					}
				}
				else {
					PlayerController->AddErrorMessage(FText::FromString("This Interior can only be placed on a Roof"), 1.5f);
				}
				break;
			case EInteriorAssetType::InteriorPlaceable:
				if (HitResult.GetActor()->IsA(AInteriorActor::StaticClass())) {
					CurrentInteriorActor->AttachToActor(HitResult.GetActor(), FAttachmentTransformRules::KeepWorldTransform);
					CurrentInteriorActor->SetActorLocation(HitResult.Location);
					CurrentInteriorActor->SetState(EInteriorActorState::Selected);
					InteriorModeState = EInteriorModeState::Free;
					if (auto* InteriorWidget = Cast<UInteriorDesignWidget>(Widget)) {
						InteriorWidget->HideScrollBox();
					}
				}
				else {
					PlayerController->AddErrorMessage(FText::FromString("This Interior can only be placed on an Interior"), 1.5f);
				}
				break;
			}
		}
	}
}

void UInteriorDesignMode::HandleInteriorNewButtonClick() {
	if (IsValid(CurrentInteriorActor)) {
		FInteriorAssetData PreviousAssetData = CurrentInteriorActor->AssetData;
		CurrentInteriorActor->SetState(EInteriorActorState::None);
		CurrentInteriorActor = nullptr;

		if (InteriorActorClass) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			CurrentInteriorActor = GetWorld()->SpawnActor<AInteriorActor>(InteriorActorClass, SpawnParams);
			BindPropertyDelegatesToActor(CurrentInteriorActor);
			CurrentInteriorActor->SetActorAssetData(PreviousAssetData);
			CurrentInteriorActor->SetState(EInteriorActorState::Preview);
			InteriorModeState = EInteriorModeState::NewObject;
			PlayerController->AddSuccessMessage(FText::FromString("Interior Preview Started"), 1.5f);
		}
	}
}

void UInteriorDesignMode::HandleInteriorDeleteButtonClick() {
	if (IsValid(CurrentInteriorActor) && CurrentInteriorActor->GetState() == EInteriorActorState::Selected) {
		CurrentInteriorActor->SetState(EInteriorActorState::None);
		CurrentInteriorActor->Destroy();
		CurrentInteriorActor = nullptr;
		PlayerController->AddSuccessMessage(FText::FromString("Interior Deleted Successfully"), 1.5f);
	}
}

void UInteriorDesignMode::HandleInteriorCloseButtonClick() {
	if (IsValid(CurrentInteriorActor)) {
		CurrentInteriorActor->SetState(EInteriorActorState::None);
		CurrentInteriorActor = nullptr;
		PlayerController->AddSuccessMessage(FText::FromString("Interior Deselected Successfully"), 1.5f);
	}
}