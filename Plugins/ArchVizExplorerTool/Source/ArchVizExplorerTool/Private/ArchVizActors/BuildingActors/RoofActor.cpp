// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizActors/BuildingActors/RoofActor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMeshGenerator.h"
#include "ArchVizUtility.h"

// Sets default values
ARoofActor::ARoofActor() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComponent);

	RoofMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Roof Mesh Component"));
	RoofMeshComponent->SetupAttachment(SceneComponent);

	RoofMeshComponent->ContainsPhysicsTriMeshData(true);
	RoofMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RoofMeshComponent->SetCollisionObjectType(ECC_WorldStatic);
	RoofMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	//RoofMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);

	//PrimaryActorTick.TickInterval = 0.1;
}

void ARoofActor::SetStartPoint(const FVector& NewStartPoint) {
	StartPoint = NewStartPoint;
}

const FVector& ARoofActor::GetStartPoint() {
	return StartPoint;
}

void ARoofActor::SetEndPoint(const FVector& NewEndPoint) {
	EndPoint = NewEndPoint;
}

const FVector& ARoofActor::GetEndPoint() {
	return EndPoint;
}

// Called when the game starts or when spawned
void ARoofActor::BeginPlay() {
	Super::BeginPlay();

}

// Called every frame
void ARoofActor::Tick(float DeltaTime) {
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

void ARoofActor::HandlePreviewState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

	SetActorLocation(HitResult.Location);
}

void ARoofActor::HandleGeneratingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

	if (EndPoint == HitResult.Location) {
		return;
	}

	double EdgeOffset{ 10.0 };

	SetEndPoint(HitResult.Location);

	double XDistance = EndPoint.X - StartPoint.X;
	double YDistance = EndPoint.Y - StartPoint.Y;

	FVector Dimensions{ abs(XDistance) + (2 * EdgeOffset), abs(YDistance) + (2 * EdgeOffset), 20 };
	FVector Offset{ Dimensions/2 };
	FVector NewStartPoint{ StartPoint };

	if (XDistance >= 0.0 && YDistance >= 0.0) {
		RoofMeshComponent->SetWorldRotation(FRotator{ 0.0 });
	}
	else if (XDistance >= 0.0 && YDistance < 0.0) {
		RoofMeshComponent->SetWorldRotation(FRotator{ 0.0,0.0,180.0 });
		Offset.Z *= -1.0;
	}
	else if (XDistance < 0.0 && YDistance >= 0.0) {
		RoofMeshComponent->SetWorldRotation(FRotator{ 180.0,0.0,0.0 });
		Offset.Z *= -1.0;
	}
	else {
		RoofMeshComponent->SetWorldRotation(FRotator{ 180.0,0.0, 180.0 });
	}

	if (XDistance >= 0.0) {
		NewStartPoint.X -= EdgeOffset;
	}
	else {
		NewStartPoint.X += EdgeOffset;
	}
	if (YDistance >= 0.0) {
		NewStartPoint.Y -= EdgeOffset;
	}
	else {
		NewStartPoint.Y += EdgeOffset;
	}

	SetActorLocation(NewStartPoint);

	GenerateRoof(Dimensions, Offset);
}

void ARoofActor::HandleMovingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

	SetActorLocation(HitResult.Location);
}

void ARoofActor::GenerateRoof(const FVector& Dimensions, const FVector& Offset) {
	DestroyRoof();
	ProceduralMeshGenerator::GenerateCube(RoofMeshComponent, 0, Dimensions, Offset);
}

void ARoofActor::DestroyRoof() {
	RoofMeshComponent->ClearAllMeshSections();
}
