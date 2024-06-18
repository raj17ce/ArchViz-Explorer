// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadActor.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARoadActor::ARoadActor() : RoadMesh{nullptr} {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComponent);

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	SplineComponent->SetupAttachment(SceneComponent);

	SplineComponent->ClearSplinePoints(true);
}

// Called when the game starts or when spawned
void ARoadActor::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void ARoadActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ARoadActor::AddSplinePoint(const FVector& Location) {

	if (!SplineComponent || !RoadMesh) return;

	SplineComponent->AddSplinePoint(Location, ESplineCoordinateSpace::World);
	GenerateRoad();
}

void ARoadActor::GenerateRoad() {

	DestroyRoadSegments();

	if (!SplineComponent || !RoadMesh) return;

	int32 SplinePointsCount = SplineComponent->GetNumberOfSplinePoints();

	if (SplinePointsCount < 2) {
		return;
	}

	double SplineLength = SplineComponent->GetSplineLength();

	double RoadMeshSizeX = 1.0;
	if (IsValid(RoadMesh)) {
		RoadMeshSizeX = ((RoadMesh->GetBoundingBox().Max - RoadMesh->GetBoundingBox().Min).X / 4);
	}

	int32 InstanceCount = FMath::FloorToInt(SplineLength / RoadMeshSizeX);

	for (int32 i = 0; i < InstanceCount; ++i) {
		FVector StartLocation = SplineComponent->GetLocationAtDistanceAlongSpline(i	* RoadMeshSizeX, ESplineCoordinateSpace::Local);
		FVector StartTangent = SplineComponent->GetTangentAtDistanceAlongSpline(i * RoadMeshSizeX, ESplineCoordinateSpace::Local);

		FVector EndLocation = SplineComponent->GetLocationAtDistanceAlongSpline(FMath::Min((i + 1) * RoadMeshSizeX, SplineLength), ESplineCoordinateSpace::Local);
		FVector EndTangent = SplineComponent->GetTangentAtDistanceAlongSpline(FMath::Min((i + 1) * RoadMeshSizeX, SplineLength), ESplineCoordinateSpace::Local);

		StartTangent = UKismetMathLibrary::ClampVectorSize(StartTangent, 0.0, RoadMeshSizeX);
		EndTangent = UKismetMathLibrary::ClampVectorSize(EndTangent, 0.0, RoadMeshSizeX);

		UE_LOG(LogTemp, Warning, TEXT("Generated segment: StartLocation=%s, StartTangent=%s, EndLocation=%s, EndTangent=%s"), *StartLocation.ToString(), *StartTangent.ToString(), *EndLocation.ToString(), *EndTangent.ToString());

		GenerateRoadSegment(StartLocation, StartTangent, EndLocation, EndTangent);
	}
}

void ARoadActor::GenerateRoadSegment(const FVector& StartLocation, const FVector& StartTangent, const FVector& EndLocation, const FVector& EndTangent) {
	auto SplineMeshComponent = NewObject<USplineMeshComponent>(this);

	if (SplineMeshComponent) {
		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		SplineMeshComponent->SetStaticMesh(RoadMesh);
		SplineMeshComponent->SetForwardAxis(ESplineMeshAxis::X);
		SplineMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
		SplineMeshComponent->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent, true);
		SplineMeshComponent->RegisterComponentWithWorld(GetWorld());
	}

	SplineMeshComponents.Add(SplineMeshComponent);
}

void ARoadActor::DestroyRoadSegments() {
	for (int32 i = 0; i < SplineMeshComponents.Num(); ++i) {
		if (SplineMeshComponents[i]) {
			SplineMeshComponents[i]->DestroyComponent();
			SplineMeshComponents[i] = nullptr;
		}
	}

	SplineMeshComponents.Empty();
}