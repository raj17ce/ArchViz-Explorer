// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizActors/BuildingActors/RoofActor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMeshGenerator.h"
#include "ArchVizUtility.h"

// Sets default values
ARoofActor::ARoofActor() : Dimensions{ 100.0,100.0, 20.0 }, Offset{ 50.0,50.0, 10.0 } {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComponent);

	RoofMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Roof Mesh Component"));
	RoofMeshComponent->SetupAttachment(SceneComponent);

	RoofMeshComponent->ContainsPhysicsTriMeshData(true);
	RoofMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RoofMeshComponent->SetCollisionObjectType(ECC_WorldDynamic);
	RoofMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	//RoofMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);

	//PrimaryActorTick.TickInterval = 0.1;
}

void ARoofActor::SetStartPoint(const FVector& NewStartPoint) {
	StartPoint = NewStartPoint;
}

const FVector& ARoofActor::GetStartPoint() const {
	return StartPoint;
}

void ARoofActor::SetEndPoint(const FVector& NewEndPoint) {
	EndPoint = NewEndPoint;
}

const FVector& ARoofActor::GetEndPoint() const {
	return EndPoint;
}

const FVector& ARoofActor::GetDimensions() const {
	return Dimensions;
}

void ARoofActor::SetDimensions(const FVector& NewDimensions) {
	Dimensions = NewDimensions;
}

const FVector& ARoofActor::GetOffset() const {
	return Offset;
}

void ARoofActor::SetOffset(const FVector& NewOffset) {
	Offset = NewOffset;
}

// Called when the game starts or when spawned
void ARoofActor::BeginPlay() {
	Super::BeginPlay();

	if (IsValid(PropertyPanelWidgetClass)) {
		PropertyPanelWidget = CreateWidget<UPropertyPanelWidget>(GetWorld(), PropertyPanelWidgetClass);
		PropertyPanelWidget->PropertyWidgetSwitcher->SetActiveWidgetIndex(3);
	}
	if (IsValid(MaterialWidgetClass)) {
		MaterialWidget = CreateWidget<UMaterialWidget>(GetWorld(), MaterialWidgetClass);
		if (IsValid(MaterialWidget->MaterialScrollBox)) {
			MaterialWidget->MaterialScrollBox->PopulateWidget(MaterialWidget->RoofMaterialDataAsset);
			MaterialWidget->MaterialScrollBox->OnItemSelected.BindUObject(this, &ARoofActor::HandleMaterialChange);
		}
	}
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

	SetEndPoint(HitResult.Location);

	double EdgeOffset{ 10.0 };

	double XDistance = EndPoint.X - StartPoint.X;
	double YDistance = EndPoint.Y - StartPoint.Y;
	double ZDistance = 20.0;

	Dimensions = FVector{ abs(XDistance) + (2 * EdgeOffset), abs(YDistance) + (2 * EdgeOffset), ZDistance };
	Offset = FVector{ abs(XDistance) / 2,  abs(YDistance) / 2, ZDistance / 2 };

	AdjustDimensionAndOffset();
	GenerateRoof();
}

void ARoofActor::HandleMovingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

	SetActorLocation(HitResult.Location);
}

void ARoofActor::GenerateRoof() {
	DestroyRoof();
	ProceduralMeshGenerator::GenerateCube(RoofMeshComponent, 0, Dimensions, Offset);

	ApplyMaterial();
}

void ARoofActor::DestroyRoof() {
	RoofMeshComponent->ClearAllMeshSections();
}

void ARoofActor::AdjustDimensionAndOffset() {
	double XDistance = EndPoint.X - StartPoint.X;
	double YDistance = EndPoint.Y - StartPoint.Y;
	double ZDistance = 20.0;

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
}

void ARoofActor::UpdateSpinBoxValue() {
	if (IsValid(PropertyPanelWidget)) {
		PropertyPanelWidget->RoofLengthSpinbox->SetValue(Dimensions.X);
		PropertyPanelWidget->RoofWidthSpinbox->SetValue(Dimensions.Y);
		PropertyPanelWidget->RoofHeightSpinbox->SetValue(Dimensions.Z);
	}
}

void ARoofActor::HandleMaterialChange(FMaterialAssetData MaterialData) {
	if (MaterialData.Material) {
		Material = MaterialData.Material;
		ApplyMaterial();
	}
}

void ARoofActor::ApplyMaterial() {
	if (IsValid(Material)) {
		if (auto* Dynamicmaterial = UMaterialInstanceDynamic::Create(Material, this)) {
			Dynamicmaterial->SetVectorParameterValue(FName("Tiling/Offset"), FLinearColor(Dimensions.X / 400.0f, Dimensions.Y / 400.0f, 0, 0));
			RoofMeshComponent->SetMaterial(0, Dynamicmaterial);
		}
	}
}
