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

	SetEndPoint(HitResult.Location);

	double XDistance = EndPoint.X - StartPoint.X;
	double YDistance = EndPoint.Y - StartPoint.Y;
	double ZDistance = 2.0;

	double EdgeOffset{ 10.0 };

	Dimensions = FVector{ abs(XDistance) + (2 * EdgeOffset), abs(YDistance) + (2 * EdgeOffset), ZDistance };
	Offset = FVector{ abs(XDistance) / 2 , abs(YDistance) / 2, ZDistance / 2 };

	GenerateFloor();
}

void AFloorActor::HandleMovingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

	SetActorLocation(HitResult.Location);
}

void AFloorActor::GenerateFloor() {
	DestroyFloor();

	FVector FloorOffset;

	AdjustOffset(FloorOffset);
	ProceduralMeshGenerator::GenerateCube(FloorMeshComponent, 0, Dimensions, FloorOffset);
	ApplyMaterial();
}

void AFloorActor::AdjustOffset(FVector& FloorOffset) {
	double XDistance = EndPoint.X - StartPoint.X;
	double YDistance = EndPoint.Y - StartPoint.Y;
	double ZDistance = 2.0;

	FloorOffset = Offset;

	if (XDistance >= 0.0 && YDistance >= 0.0) {
		FloorMeshComponent->SetWorldRotation(FRotator{ 0.0 });
	}
	else if (XDistance >= 0.0 && YDistance < 0.0) {
		FloorMeshComponent->SetWorldRotation(FRotator{ 0.0,0.0,180.0 });
		FloorOffset.Z *= -1.0;
	}
	else if (XDistance < 0.0 && YDistance >= 0.0) {
		FloorMeshComponent->SetWorldRotation(FRotator{ 180.0,0.0,0.0 });
		FloorOffset.Z *= -1.0;
	}
	else {
		FloorMeshComponent->SetWorldRotation(FRotator{ 180.0,0.0, 180.0 });
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
		ApplyMaterial();
	}
}

void AFloorActor::ApplyMaterial() {
	if (IsValid(Material)) {
		if (auto* Dynamicmaterial = UMaterialInstanceDynamic::Create(Material, this)) {
			Dynamicmaterial->SetVectorParameterValue(FName("Tiling/Offset"), FLinearColor(Dimensions.Y / 600.0f, Dimensions.X / 600.0f, 0, 0));
			FloorMeshComponent->SetMaterial(0, Dynamicmaterial);
		}
	}
}
