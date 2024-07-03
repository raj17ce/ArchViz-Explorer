// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizActors/BuildingActors/DoorActor.h"
#include "ArchVizUtility.h"
#include "ArchVizActors/BuildingActors/WallActor.h"

ADoorActor::ADoorActor() : DoorFrameStaticMesh{nullptr}, DoorStaticMesh{nullptr}, bIsOpen{false}, ParentWallComponentIndex{-1} {
	PrimaryActorTick.bCanEverTick = true;
	
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComponent);

	DoorFrameComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Frame Component"));
	DoorFrameComponent->SetupAttachment(SceneComponent);

	DoorComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Component"));
	DoorComponent->SetupAttachment(DoorFrameComponent, TEXT("Door"));

	//PrimaryActorTick.TickInterval = 0.1;
}

bool ADoorActor::GetIsOpen() const {
	return bIsOpen;
}

void ADoorActor::SetIsOpen(bool bIsDoorOpen) {
	bIsOpen = bIsDoorOpen;

	if (bIsOpen) {
		OpenDoor();
	}
	else {
		CloseDoor();
	}
}

int32 ADoorActor::GetParentWallComponentIndex() const {
	return ParentWallComponentIndex;
}

void ADoorActor::SetParentWallComponentIndex(int32 NewIndex) {
	ParentWallComponentIndex = NewIndex;
}

void ADoorActor::ShowWidget() {
	if (IsValid(PropertyPanelWidget)) {
		if (!PropertyPanelWidget->IsInViewport()) {
			PropertyPanelWidget->AddToViewport();
		}
		PropertyPanelWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ADoorActor::HideWidget() {
	if (IsValid(PropertyPanelWidget)) {
		PropertyPanelWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ADoorActor::OpenDoor() {
	if (IsValid(DoorComponent)) {
		DoorComponent->SetRelativeRotation(FRotator{ 0.0, 90.0, 0.0 });
	}
}

void ADoorActor::CloseDoor() {
	if (IsValid(DoorComponent)) {
		DoorComponent->SetRelativeRotation(FRotator{ 0.0, 0.0, 0.0 });
	}
}

void ADoorActor::BeginPlay() {
	Super::BeginPlay();

	if (DoorFrameStaticMesh && DoorStaticMesh) {
		DoorFrameComponent->SetStaticMesh(DoorFrameStaticMesh);
		DoorComponent->SetStaticMesh(DoorStaticMesh);
	}

	if (IsValid(PropertyPanelWidgetClass)) {
		PropertyPanelWidget = CreateWidget<UPropertyPanelWidget>(GetWorld(), PropertyPanelWidgetClass);
		PropertyPanelWidget->PropertyWidgetSwitcher->SetActiveWidgetIndex(1);
	}
}

void ADoorActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (State == EBuildingActorState::Preview) {
		HandlePreviewState();
	}
	else if (State == EBuildingActorState::Moving) {
		HandleMovingState();
	}
}

void ADoorActor::HandlePreviewState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});

	if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(AWallActor::StaticClass())) {
		SetActorRotation(HitResult.GetActor()->GetActorRotation());
	}

	SetActorLocation(HitResult.Location);
}

void ADoorActor::HandleMovingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});

	if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->IsA(AWallActor::StaticClass())) {
		SetActorRotation(HitResult.GetActor()->GetActorRotation());
	}

	SetActorLocation(HitResult.Location);
}
