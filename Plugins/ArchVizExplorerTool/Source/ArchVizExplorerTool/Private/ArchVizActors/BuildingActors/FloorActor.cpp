// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizActors/BuildingActors/FloorActor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMeshGenerator.h"
#include "ArchVizUtility.h"
#include "Widgets/MaterialWidget.h"

// Sets default values
AFloorActor::AFloorActor() : Dimensions{ 100.0,100.0, 2.0 }, Offset{ 50.0,50.0, 1.0 } {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComponent);

	FloorMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Floor Mesh Component"));
	FloorMeshComponent->SetupAttachment(SceneComponent);

	//PrimaryActorTick.TickInterval = 0.1;
}

void AFloorActor::SetStartPoint(const FVector& NewStartPoint) {
	StartPoint = NewStartPoint;
}

const FVector& AFloorActor::GetStartPoint() const {
	return StartPoint;
}

void AFloorActor::SetEndPoint(const FVector& NewEndPoint) {
	EndPoint = NewEndPoint;
}

const FVector& AFloorActor::GetEndPoint() const {
	return EndPoint;
}

const FVector& AFloorActor::GetDimensions() const {
	return Dimensions;
}

void AFloorActor::SetDimensions(const FVector& NewDimensions) {
	Dimensions = NewDimensions;
}

const FVector& AFloorActor::GetOffset() const {
	return Offset;
}

void AFloorActor::SetOffset(const FVector& NewOffset) {
	Offset = NewOffset;
}

// Called when the game starts or when spawned
void AFloorActor::BeginPlay() {
	Super::BeginPlay();

	if (IsValid(PropertyPanelWidgetClass)) {
		PropertyPanelWidget = CreateWidget<UPropertyPanelWidget>(GetWorld(), PropertyPanelWidgetClass);
		PropertyPanelWidget->PropertyWidgetSwitcher->SetActiveWidgetIndex(2);
	}

	if (IsValid(MaterialWidgetClass)) {
		MaterialWidget = CreateWidget<UMaterialWidget>(GetWorld(), MaterialWidgetClass);
		if (IsValid(MaterialWidget->MaterialScrollBox)) {
			MaterialWidget->MaterialScrollBox->PopulateWidget(MaterialWidget->FloorMaterialDataAsset);
			MaterialWidget->MaterialScrollBox->OnItemSelected.BindUObject(this, &AFloorActor::HandleMaterialChange);
		}
	}
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

	double EdgeOffset{ 10.0 };

	SetEndPoint(HitResult.Location);

	double XDistance = EndPoint.X - StartPoint.X;
	double YDistance = EndPoint.Y - StartPoint.Y;
	double ZDistance = 2.0;

	FVector NewDimensions{ abs(XDistance) + (2 * EdgeOffset), abs(YDistance) + (2 * EdgeOffset), ZDistance };
	FVector NewOffset{ abs(XDistance) / 2 , abs(YDistance) / 2, ZDistance / 2};

	if (XDistance >= 0.0 && YDistance >= 0.0) {
		FloorMeshComponent->SetWorldRotation(FRotator{ 0.0 });
	}
	else if (XDistance >= 0.0 && YDistance < 0.0) {
		FloorMeshComponent->SetWorldRotation(FRotator{ 0.0,0.0,180.0 });
		NewOffset.Z *= -1.0;
	}
	else if (XDistance < 0.0 && YDistance >= 0.0) {
		FloorMeshComponent->SetWorldRotation(FRotator{ 180.0,0.0,0.0 });
		NewOffset.Z *= -1.0;
	}
	else {
		FloorMeshComponent->SetWorldRotation(FRotator{ 180.0,0.0, 180.0 });
	}

	Dimensions = NewDimensions;
	Offset = NewOffset;

	GenerateFloor();
}

void AFloorActor::HandleMovingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

	SetActorLocation(HitResult.Location);
}

void AFloorActor::GenerateFloor() {
	DestroyFloor();
	ProceduralMeshGenerator::GenerateCube(FloorMeshComponent, 0, Dimensions, Offset);

	if (IsValid(Material)) {
		FloorMeshComponent->SetMaterial(0, Material);
	}
}

void AFloorActor::DestroyFloor() {
	FloorMeshComponent->ClearAllMeshSections();
}

void AFloorActor::UpdateSpinBoxValue() {
	if (IsValid(PropertyPanelWidget)) {
		PropertyPanelWidget->FloorLengthSpinbox->SetValue(Dimensions.X);
		PropertyPanelWidget->FloorWidthSpinbox->SetValue(Dimensions.Y);
		PropertyPanelWidget->FloorHeightSpinbox->SetValue(Dimensions.Z);
	}
}

void AFloorActor::HandleMaterialChange(FMaterialAssetData MaterialData) {
	if (MaterialData.Material) {
		Material = MaterialData.Material;
		FloorMeshComponent->SetMaterial(0, MaterialData.Material);
	}
}
