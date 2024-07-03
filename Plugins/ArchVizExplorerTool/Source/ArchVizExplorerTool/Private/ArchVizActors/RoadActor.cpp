// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizActors/RoadActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "ArchVizController.h"

// Sets default values
ARoadActor::ARoadActor() : RoadMesh{ nullptr }, Width{ 0.0 }, State{ ERoadActorState::None }, RoadType{ ERoadType::Curved } {
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

	if (IsValid(PropertyPanelWidgetClass)) {
		PropertyPanelWidget = CreateWidget<UPropertyPanelWidget>(GetWorld(), PropertyPanelWidgetClass);
		PropertyPanelWidget->PropertyWidgetSwitcher->SetActiveWidgetIndex(5);
		Width = PropertyPanelWidget->RoadWidthSpinbox->GetValue();
	}

	if (IsValid(MaterialWidgetClass)) {
		MaterialWidget = CreateWidget<UMaterialWidget>(GetWorld(), MaterialWidgetClass);
		if (IsValid(MaterialWidget->MaterialScrollBox)) {
			MaterialWidget->MaterialScrollBox->PopulateWidget(MaterialWidget->RoadMaterialDataAsset);
			MaterialWidget->MaterialScrollBox->OnItemSelected.BindUObject(this, &ARoadActor::HandleMaterialChange);
		}
	}
}

// Called every frame
void ARoadActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

TArray<FVector> ARoadActor::GetSplinePoints() const {
	return SplinePoints;
}

void ARoadActor::SetSplinePoints(const TArray<FVector>& NewSplinePoints) {
	SplinePoints = NewSplinePoints;
}

float ARoadActor::GetWidth() const {
	return Width;
}

void ARoadActor::SetWidth(float NewWidth) {
	Width = NewWidth;
}

void ARoadActor::AddSplinePoint(const FVector& Location) {

	if (!IsValid(SplineComponent) || !IsValid(RoadMesh) || State != ERoadActorState::Generating) return;

	if (!SplinePoints.IsEmpty()) {
		if (UKismetMathLibrary::Vector_Distance(Location, SplinePoints[SplinePoints.Num() - 1]) < 400) {
			return;
		}
	}

	SplinePoints.Add(Location);
	UpdateRoad();
}

bool ARoadActor::RemoveLastSplinePoint() {
	if (State != ERoadActorState::Generating) return false;

	if (!SplinePoints.IsEmpty()) {
		SplinePoints.RemoveAt(SplinePoints.Num() - 1);
		UpdateRoad();
		return true;
	}

	return false;
}

void ARoadActor::UpdateRoad() {
	if (SplinePoints.Num() < 2) {
		DestroyRoadSegments();
		return;
	}

	SplineComponent->SetSplinePoints(SplinePoints, ESplineCoordinateSpace::World);

	const int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();
	const FVector MeshBounds = RoadMesh->GetBounds().BoxExtent * 2.0f;
	const float MeshWidth = MeshBounds.Y;
	const float MeshLength = MeshBounds.X;
	const float ScaleFactor = Width / MeshWidth;
	//const float ScaleFactor = 1.0;

	if (RoadType == ERoadType::Sharp) {
		for (int32 i = 0; i < NumPoints; ++i) {
			SplineComponent->SetSplinePointType(i, ESplinePointType::CurveClamped, true);
		}
	}

	int32 SegmentIndex = 0;

	for (int32 PointIndex = 0; PointIndex < NumPoints - 1; ++PointIndex) {
		const float StartDistance = SplineComponent->GetDistanceAlongSplineAtSplinePoint(PointIndex);
		const float EndDistance = SplineComponent->GetDistanceAlongSplineAtSplinePoint(PointIndex + 1);
		const float SegmentLength = EndDistance - StartDistance;
		const int32 NumberOfSegments = FMath::CeilToInt(SegmentLength / MeshLength);

		for (int32 ComponentIndex = 0; ComponentIndex < NumberOfSegments; ++ComponentIndex) {
			if (SegmentIndex >= SplineMeshComponents.Num()) {
				auto* SplineMeshComponent = NewObject<USplineMeshComponent>(this);
				SplineMeshComponent->SetMobility(EComponentMobility::Movable);
				SplineMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
				SplineMeshComponent->SetStaticMesh(RoadMesh);
				SplineMeshComponent->RegisterComponent();

				SplineMeshComponents.Add(SplineMeshComponent);
			}

			auto* SplineMeshComponent = SplineMeshComponents[SegmentIndex];
			SplineMeshComponent->SetVisibility(true);
			float StretchedMeshLength = SegmentLength / NumberOfSegments;

			float LocalStartDistance = StartDistance + ComponentIndex * StretchedMeshLength - 200;
			float LocalEndDistance = StartDistance + (ComponentIndex + 1) * StretchedMeshLength - 200;

			const FVector StartLocation = SplineComponent->GetLocationAtDistanceAlongSpline(LocalStartDistance, ESplineCoordinateSpace::Local);
			const FVector StartTangent = SplineComponent->GetTangentAtDistanceAlongSpline(LocalStartDistance, ESplineCoordinateSpace::Local).GetClampedToMaxSize(MeshLength);
			const FVector EndLocation = SplineComponent->GetLocationAtDistanceAlongSpline(LocalEndDistance, ESplineCoordinateSpace::Local);
			const FVector EndTangent = SplineComponent->GetTangentAtDistanceAlongSpline(LocalEndDistance, ESplineCoordinateSpace::Local).GetClampedToMaxSize(MeshLength);

			SplineMeshComponent->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent, true);
			SplineMeshComponent->SetStartScale(FVector2D(ScaleFactor, 1));
			SplineMeshComponent->SetEndScale(FVector2D(ScaleFactor, 1));
			SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			SplineMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);

			if (IsValid(Material)) {
				SplineMeshComponent->SetMaterial(0, Material);
			}

			++SegmentIndex;
		}
	}

	for (int32 ExtraSegmentsIndex = SegmentIndex; ExtraSegmentsIndex < SplineMeshComponents.Num(); ++ExtraSegmentsIndex) {
		SplineMeshComponents[ExtraSegmentsIndex]->SetVisibility(false);
	}
}

void ARoadActor::UpdatePropertyPanelValues() {
	if (IsValid(PropertyPanelWidget)) {
		PropertyPanelWidget->RoadWidthSpinbox->SetValue(Width);
		
		switch (RoadType) {
		case ERoadType::Sharp:
			PropertyPanelWidget->RoadTypeComboBox->SetSelectedOption("Sharp");
			break;
		case ERoadType::Curved:
			PropertyPanelWidget->RoadTypeComboBox->SetSelectedOption("Curved");
			break;
		}
	}
}

void ARoadActor::SetState(ERoadActorState NewState) {
	State = NewState;

	HandleStateChange();
}

ERoadActorState ARoadActor::GetState() const {
	return State;
}

void ARoadActor::SetRoadType(ERoadType NewRoadType) {
	RoadType = NewRoadType;
	UpdateRoad();
}

ERoadType ARoadActor::GetRoadType() const {
	return RoadType;
}

void ARoadActor::HandleStateChange() {

	OnRoadActorStateChanged.ExecuteIfBound(State);

	if (State == ERoadActorState::Selected) {
		ShowWidget();
		HighlightSelectedActor();
	}
	else {
		HideWidget();
		UnhighlightDeselectedActor();
	}
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

void ARoadActor::HandleMaterialChange(FMaterialAssetData MaterialData) {
	if (IsValid(MaterialData.Material)) {
		Material = MaterialData.Material;
		UpdateRoad();
		if (auto* ArchVizController = Cast<AArchVizController>(GetWorld()->GetFirstPlayerController())) {
			ArchVizController->AddSuccessMessage(FText::FromString("Material Applied Successfully"), 1.5f);
		}
	}
}
