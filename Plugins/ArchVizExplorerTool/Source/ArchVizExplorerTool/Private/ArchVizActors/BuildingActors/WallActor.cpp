// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizActors/BuildingActors/WallActor.h"
#include "ArchVizUtility.h"

// Sets default values
AWallActor::AWallActor() : WallStaticMesh{nullptr}, State{EWallActorState::Preview} {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComponent);
	SceneComponent->SetMobility(EComponentMobility::Movable);

	PrimaryActorTick.TickInterval = 0.3;
}

// Called when the game starts or when spawned
void AWallActor::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void AWallActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (State == EWallActorState::Preview) {
		FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
		HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

		SetActorLocation(HitResult.Location);
	}
	else if (State == EWallActorState::Moving) {
		FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
		HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

		SetActorLocation(HitResult.Location);
	}
	else if (State == EWallActorState::Generating) {
		FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
		HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);
		SetEndLocation(HitResult.Location);

		double XDistance = EndLocation.X - StartLocation.X;
		double YDistance = EndLocation.Y - StartLocation.Y;

		if (EndLocation != StartLocation) {
			if (abs(XDistance) >= abs(YDistance)) {
				GenerateWallSegments(abs(XDistance));
				
				if (XDistance >= 0) {
					SetActorRotation(FRotator{ 0.0 });
				}
				else {
					SetActorRotation(FRotator{0.0, 180.0, 0.0});
				}
			}
			else {
				GenerateWallSegments(abs(YDistance));

				if (YDistance >= 0) {
					SetActorRotation(FRotator{ 0.0, 90.0, 0.0 });
				}
				else {
					SetActorRotation(FRotator{ 0.0, 270.0, 0.0 });
				}
			}
		}
	}
}

void AWallActor::GenerateWallSegments(double Length) {
	DestroyWallSegments();

	int32 NumberOfSegments{};

	FVector WallSize{};
	if (IsValid(WallStaticMesh)) {
		WallSize = WallStaticMesh->GetBoundingBox().GetSize();
		NumberOfSegments = FMath::FloorToInt32(Length / WallSize.X) + 1;
	}

	for (int32 SegmentIndex = 0; SegmentIndex < NumberOfSegments; ++SegmentIndex) {
		UStaticMeshComponent* WallMeshComponent = NewObject<UStaticMeshComponent>(this);
		WallMeshComponent->AttachToComponent(SceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
		WallMeshComponent->RegisterComponentWithWorld(GetWorld());
		WallMeshComponent->SetMobility(EComponentMobility::Movable);

		if (IsValid(WallStaticMesh)) {
			WallMeshComponent->SetStaticMesh(WallStaticMesh);
			WallSegments.Add(WallMeshComponent);
		}

		WallMeshComponent->SetRelativeLocation(FVector{SegmentIndex * WallSize.X, 0, 0});
	}
}

void AWallActor::DestroyWallSegments() {
	for (auto& Segment : WallSegments) {
		Segment->DestroyComponent();
		Segment = nullptr;
	}

	WallSegments.Empty();
}

void AWallActor::SetStartLocation(const FVector& NewStartLocation) {
	StartLocation = NewStartLocation;
}

const FVector& AWallActor::GetStartLocation() const{
	return StartLocation;
}

void AWallActor::SetEndLocation(const FVector& NewEndLocation) {
	EndLocation = NewEndLocation;
}

const FVector& AWallActor::GetEndLocation() const {
	return EndLocation;
}

void AWallActor::SetState(EWallActorState NewState) {
	State = NewState;
}

EWallActorState AWallActor::GetState() const {
	return State;
}