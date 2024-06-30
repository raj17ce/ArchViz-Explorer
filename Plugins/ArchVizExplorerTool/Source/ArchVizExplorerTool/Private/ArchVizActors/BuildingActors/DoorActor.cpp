// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizActors/BuildingActors/DoorActor.h"
#include "ArchVizUtility.h"
#include "ArchVizActors/BuildingActors/WallActor.h"

ADoorActor::ADoorActor() : DoorFrameStaticMesh{nullptr}, DoorStaticMesh{nullptr} {
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComponent);

	DoorFrameComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Frame Component"));
	DoorFrameComponent->SetupAttachment(SceneComponent);

	DoorComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Component"));
	DoorComponent->SetupAttachment(DoorFrameComponent, TEXT("Door"));

	//PrimaryActorTick.TickInterval = 0.1;
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
