// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizActors/BuildingActors/WallActor.h"
#include "ArchVizUtility.h"

// Sets default values
AWallActor::AWallActor() : WallSegmentIndex{ -1 } {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComponent);
	SceneComponent->SetMobility(EComponentMobility::Movable);

	PrimaryActorTick.TickInterval = 0.1;
}

// Called when the game starts or when spawned
void AWallActor::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void AWallActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (bShowPreview) {
		FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
		HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);
		//SetActorLocation(HitResult.Location);
		SetEndLocation(HitResult.Location);

		if(EndLocation != StartLocation) {
			GenerateWallSegments(EndLocation.X - StartLocation.X);
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
		UStaticMeshComponent* WallMeshComponent = NewObject<UStaticMeshComponent>();
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

void AWallActor::SetEndLocation(const FVector& NewEndLocation) {
	EndLocation = NewEndLocation;
}

void AWallActor::SetShowPreview(bool NewShowPreview) {
	bShowPreview = NewShowPreview;
}

int32 AWallActor::GetSegmentIndex() const {
	return WallSegmentIndex;
}

void AWallActor::SetSegmentIndex(int32 Index) {
	WallSegmentIndex = Index;
}