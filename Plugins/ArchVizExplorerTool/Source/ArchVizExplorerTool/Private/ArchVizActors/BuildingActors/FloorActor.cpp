// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizActors/BuildingActors/FloorActor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMeshGenerator.h"
#include "ArchVizUtility.h"

// Sets default values
AFloorActor::AFloorActor() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComponent);

	FloorMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Floor Mesh Component"));
	FloorMeshComponent->SetupAttachment(SceneComponent);

	PrimaryActorTick.TickInterval = 0.3;
}

void AFloorActor::SetStartPoint(const FVector& NewStartPoint) {
	StartPoint = NewStartPoint;
}

const FVector& AFloorActor::GetStartPoint() {
	return StartPoint;
}

void AFloorActor::SetEndPoint(const FVector& NewEndPoint) {
	EndPoint = NewEndPoint;
}

const FVector& AFloorActor::GetEndPoint() {
	return EndPoint;
}

// Called when the game starts or when spawned
void AFloorActor::BeginPlay() {
	Super::BeginPlay();

}

// Called every frame
void AFloorActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (State == EBuildingActorState::Preview) {
		HandlePreviewState();
	}
	else if (State == EBuildingActorState::Moving) {
		HandleMovingState();
	}
	else if (State == EBuildingActorState::Generating) {
		HandleGeneratingState();
	}
}

void AFloorActor::HandlePreviewState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

	SetActorLocation(HitResult.Location);
}

void AFloorActor::HandleGeneratingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

	if (EndPoint == HitResult.Location) {
		return;
	}

	SetEndPoint(HitResult.Location);

	double XDistance = EndPoint.X - StartPoint.X;
	double YDistance = EndPoint.Y - StartPoint.Y;

	FVector Dimensions{abs(XDistance), abs(YDistance), 10};

	FVector Offset{ XDistance/2, YDistance/2, 0.0};

	GenerateFloor(Dimensions, Offset);
}

void AFloorActor::HandleMovingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

	SetActorLocation(HitResult.Location);
}

void AFloorActor::GenerateFloor(const FVector& Dimensions, const FVector& Offset) {
	DestroyFloor();
	ProceduralMeshGenerator::GenerateCube(FloorMeshComponent, 0, Dimensions, Offset);
}

void AFloorActor::DestroyFloor() {
	FloorMeshComponent->ClearAllMeshSections();
}
