// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizController.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputCoreTypes.h"
#include "EnhancedInputSubsystems.h"

AArchVizController::AArchVizController() : RoadMappingContext{ nullptr }, RoadActor{ nullptr }, ControllerModeWidget{ nullptr }, RoadWidget{ nullptr }, BuildingWidget{ nullptr }, InteriorWidget{ nullptr } {

}

void AArchVizController::BeginPlay() {
	Super::BeginPlay();

	if (IsValid(ControllerModeWidgetClass)) {
		ControllerModeWidget = CreateWidget<UControllerModeWidget>(this, ControllerModeWidgetClass, "Controller Mode Widget");
		ControllerModeWidget->OnControllerModeChange.AddUObject(this, &AArchVizController::HandleControllerModeChange);
		ControllerModeWidget->AddToViewport(1);

		if (IsValid(RoadWidgetClass)) {
			RoadWidget = CreateWidget<URoadConstructionWidget>(this, RoadWidgetClass, "Road Widget");
			RoadWidget->AddToViewport();
		}

		if (IsValid(BuildingWidgetClass)) {
			BuildingWidget = CreateWidget<UBuildingConstructionWidget>(this, BuildingWidgetClass, "Building Widget");
		}

		if (IsValid(InteriorWidgetClass)) {
			InteriorWidget = CreateWidget<UInteriorDesignWidget>(this, InteriorWidgetClass, "Interior Widget");
		}
	}

	SetInputMode(InputModeGameAndUI);
	SetShowMouseCursor(true);
}

void AArchVizController::SetupInputComponent() {

	Super::SetupInputComponent();

	if (auto* EIC = Cast<UEnhancedInputComponent>(InputComponent)) {

		RoadMappingContext = NewObject<UInputMappingContext>();

		auto* RoadLeftMouseClickAction = NewObject<UInputAction>();
		RoadLeftMouseClickAction->ValueType = EInputActionValueType::Boolean;

		RoadMappingContext->MapKey(RoadLeftMouseClickAction, EKeys::LeftMouseButton);
		EIC->BindAction(RoadLeftMouseClickAction, ETriggerEvent::Completed, this, &AArchVizController::HandleRoadLeftMouseClick);

		if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
			LocalPlayerSubsystem->AddMappingContext(RoadMappingContext, 0);
		}
	}
}

void AArchVizController::HandleRoadLeftMouseClick() {
	FHitResult HitResult{};
	GetHitResultUnderCursorByChannel(TraceTypeQuery1, true, HitResult);

	if (IsValid(RoadActorClass) && !IsValid(RoadActor)) {
		RoadActor = NewObject<ARoadActor>(this, RoadActorClass);
	}

	RoadActor->AddSplinePoint(HitResult.Location);
}

void AArchVizController::HandleControllerModeChange(EControllerMode NewControllerMode) {
	UpdateMappingContext(NewControllerMode);
	UpdateUI(NewControllerMode);
}

void AArchVizController::UpdateUI(EControllerMode NewControllerMode) {
	switch (NewControllerMode) {
	case EControllerMode::RoadConstruction:
		if (IsValid(BuildingWidget) && BuildingWidget->IsInViewport()) {
			BuildingWidget->RemoveFromParent();
		}
		if (IsValid(InteriorWidget) && InteriorWidget->IsInViewport()) {
			InteriorWidget->RemoveFromParent();
		}
		if (IsValid(RoadWidget)) {
			RoadWidget->AddToViewport();
		}
		break;
	case EControllerMode::BuildingConstruction:
		if (IsValid(RoadWidget) && RoadWidget->IsInViewport()) {
			RoadWidget->RemoveFromParent();
		}
		if (IsValid(InteriorWidget) && InteriorWidget->IsInViewport()) {
			InteriorWidget->RemoveFromParent();
		}
		if (IsValid(BuildingWidget)) {
			BuildingWidget->AddToViewport();
		}
		break;
	case EControllerMode::InteriorDesign:
		if (IsValid(RoadWidget) && RoadWidget->IsInViewport()) {
			RoadWidget->RemoveFromParent();
		}
		if (IsValid(BuildingWidget) && BuildingWidget->IsInViewport()) {
			BuildingWidget->RemoveFromParent();
		}
		if (IsValid(InteriorWidget)) {
			InteriorWidget->AddToViewport();
		}
		break;
	}
}

void AArchVizController::UpdateMappingContext(EControllerMode NewControllerMode) {
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, "Mapping Context Changed");
}

FVector AArchVizController::SnapToGrid(FVector WorldLocation) {
	float GridSpacing = 100.0f;
	float SnapX = FMath::RoundToFloat(WorldLocation.X / GridSpacing) * GridSpacing;
	float SnapY = FMath::RoundToFloat(WorldLocation.Y / GridSpacing) * GridSpacing;
	float SnapZ = FMath::RoundToFloat(WorldLocation.Z / GridSpacing) * GridSpacing;

	return FVector(SnapX, SnapY, SnapZ);
}