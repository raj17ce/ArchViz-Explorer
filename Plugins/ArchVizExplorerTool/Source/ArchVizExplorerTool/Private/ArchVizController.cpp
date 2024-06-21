// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizController.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputCoreTypes.h"
#include "EnhancedInputSubsystems.h"
#include "ArchVizUtility.h"

AArchVizController::AArchVizController() : CurrentArchVizMode{EArchVizMode::RoadConstruction}, CurrentArchVizModePtr{nullptr}, RoadConstructionMode{nullptr}, BuildingConstructionMode{nullptr}, InteriorDesignMode{nullptr}, ArchVizModeWidget{nullptr}, RoadWidget{nullptr}, BuildingWidget{nullptr}, InteriorWidget{nullptr} {

}

void AArchVizController::BeginPlay() {
	Super::BeginPlay();

	if (RoadConstructionModeClass) {
		RoadConstructionMode = NewObject<URoadConstructionMode>(this, RoadConstructionModeClass);
		RoadConstructionMode->Setup();
		RoadConstructionMode->SetPlayerController(this);
		RoadConstructionMode->SetupInputComponent();
	}
	if (BuildingConstructionModeClass) {
		BuildingConstructionMode = NewObject<UBuildingConstructionMode>(this, BuildingConstructionModeClass);
		BuildingConstructionMode->Setup();
		BuildingConstructionMode->SetPlayerController(this);
		BuildingConstructionMode->SetupInputComponent();
	}
	if (InteriorDesignModeClass) {
		InteriorDesignMode = NewObject<UInteriorDesignMode>(this, InteriorDesignModeClass);
		InteriorDesignMode->Setup();
		InteriorDesignMode->SetPlayerController(this);
		InteriorDesignMode->SetupInputComponent();
	}

	if (RoadConstructionMode) {
		SetArchVizMode(RoadConstructionMode);
	}

	if (IsValid(ArchVizModeWidgetClass)) {
		ArchVizModeWidget = CreateWidget<UArchVizModeWidget>(this, ArchVizModeWidgetClass, "Controller Mode Widget");
		ArchVizModeWidget->OnArchVizModeChange.AddUObject(this, &AArchVizController::HandleArchVizModeChange);
		ArchVizModeWidget->AddToViewport(1);

		if (IsValid(RoadWidgetClass)) {
			RoadWidget = CreateWidget<URoadConstructionWidget>(this, RoadWidgetClass, "Road Widget");
			RoadWidget->AddToViewport();
		}

		if (IsValid(BuildingWidgetClass)) {
			BuildingWidget = CreateWidget<UBuildingConstructionWidget>(this, BuildingWidgetClass, "Building Widget");
			BuildingWidget->OnBuildingSubModeChange.AddUObject(BuildingConstructionMode, &UBuildingConstructionMode::HandleBuildingSubModeChange);
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

	//if (CurrentArchVizMode == EArchVizMode::BuildingConstruction) {
	//	/*if (IsValid(WallActorClass) && !IsValid(WallActor)) {
	//		WallActor = NewObject<AWallActor>(this, WallActorClass);
	//	}*/
	//	auto* WallActor = BuildingConstructionMode->GetWallActor();

	//	if (IsValid(WallActor) && !IsValid(WallActor->PreviewWallSegment)) {
	//		WallActor->PreviewWallSegment = NewObject<UStaticMeshComponent>();
	//		WallActor->PreviewWallSegment->RegisterComponentWithWorld(GetWorld());
	//	}

	//	FHitResult HitResult{};
	//	GetHitResultUnderCursorByChannel(TraceTypeQuery1, true, HitResult);
	//	HitResult.Location.Z = 0.0;

	//	if (IsValid(WallActor->PreviewWallSegment) && IsValid(WallActor->WallStaticMesh)) {
	//		WallActor->PreviewWallSegment->SetStaticMesh(WallActor->WallStaticMesh);

	//		WallActor->PreviewWallSegment->SetWorldLocation(ArchVizUtility::GetSnappedLocation(HitResult.Location));
	//		WallActor->PreviewWallSegment->SetWorldRotation(WallActor->GetSegmentRotation());
	//	}
	//}
}

void AArchVizController::SetupInputComponent() {
	Super::SetupInputComponent();
}

void AArchVizController::HandleArchVizModeChange(EArchVizMode NewArchVizMode) {
	CurrentArchVizMode = NewArchVizMode;

	UpdateArchVizMode();
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

void AArchVizController::UpdateArchVizMode() {
	switch (CurrentArchVizMode) {
	case EArchVizMode::RoadConstruction:
		SetArchVizMode(RoadConstructionMode);
		break;
	case EArchVizMode::BuildingConstruction:
		SetArchVizMode(BuildingConstructionMode);
		break;
	case EArchVizMode::InteriorDesign:
		SetArchVizMode(InteriorDesignMode);
		break;
	}
}

void AArchVizController::SetArchVizMode(IArchVizMode* NewArchVizModePtr) {
	if (CurrentArchVizModePtr) {
		CurrentArchVizModePtr->ExitMode();
	}

	CurrentArchVizModePtr = NewArchVizModePtr;

	if (CurrentArchVizModePtr) {
		CurrentArchVizModePtr->EnterMode();
	}
}