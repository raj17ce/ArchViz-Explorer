// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizActors/BuildingActors/DoorActor.h"
#include "ArchVizUtility.h"
#include "ArchVizActors/BuildingActors/WallActor.h"

ADoorActor::ADoorActor() : DoorFrameStaticMesh{nullptr}, DoorStaticMesh{nullptr} {
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComponent);
	SceneComponent->SetMobility(EComponentMobility::Movable);

	DoorFrameComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Frame Component"));
	DoorFrameComponent->SetupAttachment(SceneComponent);

	DoorComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Component"));
	DoorComponent->SetupAttachment(DoorFrameComponent, TEXT("Door"));
}

void ADoorActor::BeginPlay() {
	Super::BeginPlay();

}

void ADoorActor::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	if (DoorFrameStaticMesh && DoorStaticMesh) {
		DoorFrameComponent->SetStaticMesh(DoorFrameStaticMesh);
		DoorComponent->SetStaticMesh(DoorStaticMesh);
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
	//HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

	if (HitResult.GetActor() && HitResult.GetActor()->IsA(AWallActor::StaticClass())) {
		SetActorRotation(HitResult.GetActor()->GetActorRotation());

		// TODO - Replace Static Mesh and Attach to Socket
		//AWallActor* WallActor = Cast<AWallActor>(HitResult.GetActor());
	}

	SetActorLocation(HitResult.Location);
}

void ADoorActor::HandleGeneratingState() {

}

void ADoorActor::HandleMovingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	//HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

	if (HitResult.GetActor() && HitResult.GetActor()->IsA(AWallActor::StaticClass())) {
		SetActorRotation(HitResult.GetActor()->GetActorRotation());

		// TODO - Replace Static Mesh and Attach to Socket
		//AWallActor* WallActor = Cast<AWallActor>(HitResult.GetActor());
	}

	SetActorLocation(HitResult.Location);
}
