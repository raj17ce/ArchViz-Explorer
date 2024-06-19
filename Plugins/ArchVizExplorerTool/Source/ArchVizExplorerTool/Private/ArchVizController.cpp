// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizController.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputCoreTypes.h"
#include "EnhancedInputSubsystems.h"

AArchVizController::AArchVizController() : CurrentArchVizMode{EArchVizMode::RoadConstruction}, RoadMappingContext{nullptr}, RoadActor{nullptr}, WallMappingContext{nullptr}, WallActor{nullptr}, ArchVizModeWidget{nullptr}, RoadWidget{nullptr}, BuildingWidget{nullptr}, InteriorWidget{nullptr} {

}

void AArchVizController::BeginPlay() {
	Super::BeginPlay();

	if (IsValid(ArchVizModeWidgetClass)) {
		ArchVizModeWidget = CreateWidget<UArchVizModeWidget>(this, ArchVizModeWidgetClass, "Controller Mode Widget");
		ArchVizModeWidget->OnArchVizModeChange.AddUObject(this, &AArchVizController::HandleControllerModeChange);
		ArchVizModeWidget->AddToViewport(1);

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

void AArchVizController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (CurrentArchVizMode == EArchVizMode::BuildingConstruction) {
		if (IsValid(WallActorClass) && !IsValid(WallActor)) {
			WallActor = NewObject<AWallActor>(this, WallActorClass);
		}
		if (IsValid(WallActor) && !IsValid(WallActor->PreviewWallSegment)) {
			WallActor->PreviewWallSegment = NewObject<UStaticMeshComponent>();
			WallActor->PreviewWallSegment->RegisterComponentWithWorld(GetWorld());
		}
		FHitResult HitResult = GetHitResult();
		HitResult.Location.Z = 0.0;

		if (IsValid(WallActor->PreviewWallSegment) && IsValid(WallActor->WallStaticMesh)) {
			WallActor->PreviewWallSegment->SetStaticMesh(WallActor->WallStaticMesh);

			WallActor->PreviewWallSegment->SetWorldLocation(SnapToGrid(HitResult.Location));
			WallActor->PreviewWallSegment->SetWorldRotation(WallActor->GetSegmentRotation());
		}
	}
}

void AArchVizController::SetupInputComponent() {

	Super::SetupInputComponent();

	SetupRoadInputComponent();
	SetupWallInputComponent();

	if (auto* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		LocalPlayerSubsystem->AddMappingContext(RoadMappingContext, 0);
	}
}

void AArchVizController::SetupRoadInputComponent() {
	if (auto* EIC = Cast<UEnhancedInputComponent>(InputComponent)) {

		RoadMappingContext = NewObject<UInputMappingContext>();

		//Left-Click
		auto* RoadLeftMouseClickAction = NewObject<UInputAction>();
		RoadLeftMouseClickAction->ValueType = EInputActionValueType::Boolean;

		RoadMappingContext->MapKey(RoadLeftMouseClickAction, EKeys::LeftMouseButton);
		EIC->BindAction(RoadLeftMouseClickAction, ETriggerEvent::Completed, this, &AArchVizController::HandleRoadLeftMouseClick);
	}
}

void AArchVizController::SetupWallInputComponent() {
	if (auto* EIC = Cast<UEnhancedInputComponent>(InputComponent)) {

		WallMappingContext = NewObject<UInputMappingContext>();

		//Left-Click
		auto* WallLeftMouseClickAction = NewObject<UInputAction>();
		WallLeftMouseClickAction->ValueType = EInputActionValueType::Boolean;

		WallMappingContext->MapKey(WallLeftMouseClickAction, EKeys::LeftMouseButton);
		EIC->BindAction(WallLeftMouseClickAction, ETriggerEvent::Completed, this, &AArchVizController::HandleWallLeftMouseClick);

		//R-Key
		auto* WallRKeyPressAction = NewObject<UInputAction>(this);
		WallRKeyPressAction->ValueType = EInputActionValueType::Boolean;

		WallMappingContext->MapKey(WallRKeyPressAction, EKeys::R);
		EIC->BindAction(WallRKeyPressAction, ETriggerEvent::Completed, this, &AArchVizController::HandleWallRKeyPress);
	}
}

void AArchVizController::HandleRoadLeftMouseClick() {
	FHitResult HitResult = GetHitResult();

	if (IsValid(RoadActorClass) && !IsValid(RoadActor)) {
		RoadActor = NewObject<ARoadActor>(this, RoadActorClass);
	}

	RoadActor->AddSplinePoint(HitResult.Location);
}

void AArchVizController::HandleWallLeftMouseClick() {
	auto* NewWallSegment = NewObject<UStaticMeshComponent>();

	if (IsValid(WallActor) && IsValid(WallActor->WallStaticMesh)) {
		NewWallSegment->SetStaticMesh(WallActor->WallStaticMesh);

		NewWallSegment->RegisterComponentWithWorld(GetWorld());
		NewWallSegment->SetWorldLocation(WallActor->PreviewWallSegment->GetComponentLocation());
		NewWallSegment->SetWorldRotation(WallActor->GetSegmentRotation());

		WallActor->WallSegments.Add(NewWallSegment);
		WallActor->SetSegmentIndex(WallActor->GetSegmentIndex() + 1);
	}
}

void AArchVizController::HandleWallRKeyPress() {
	if (IsValid(WallActor)) {
		double NewYaw = (WallActor->GetSegmentRotation().Yaw + 90);
		if (NewYaw >= 360) {
			NewYaw -= 360;
		}
		WallActor->SetSegmentRotation(FRotator{ 0.0, NewYaw, 0.0 });
	}
}

void AArchVizController::HandleControllerModeChange(EArchVizMode NewArchVizMode) {
	CurrentArchVizMode = NewArchVizMode;

	UpdateMappingContext();
	UpdateUI();
}

void AArchVizController::UpdateUI() {
	switch (CurrentArchVizMode) {
	case EArchVizMode::RoadConstruction:
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
	case EArchVizMode::BuildingConstruction:
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
	case EArchVizMode::InteriorDesign:
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

void AArchVizController::UpdateMappingContext() {
	
	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {

		Subsystem->ClearAllMappings();

		switch (CurrentArchVizMode) {
		case EArchVizMode::RoadConstruction:
			Subsystem->AddMappingContext(RoadMappingContext, 0);
			break;
		case EArchVizMode::BuildingConstruction:
			Subsystem->AddMappingContext(WallMappingContext, 0);
			break;
		case EArchVizMode::InteriorDesign:
			break;
		}
	}
}

FVector AArchVizController::SnapToGrid(FVector WorldLocation) {
	float GridSpacing = 100.0f;
	float SnapX = FMath::RoundToFloat(WorldLocation.X / GridSpacing) * GridSpacing;
	float SnapY = FMath::RoundToFloat(WorldLocation.Y / GridSpacing) * GridSpacing;
	float SnapZ = FMath::RoundToFloat(WorldLocation.Z / GridSpacing) * GridSpacing;

	return FVector(SnapX, SnapY, SnapZ);
}

FHitResult AArchVizController::GetHitResult() const {
	FHitResult MouseHitResult{};
	GetHitResultUnderCursorByChannel(TraceTypeQuery1, true, MouseHitResult);

	return MouseHitResult;
}