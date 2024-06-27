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

	//PrimaryActorTick.TickInterval = 0.1;
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

	if (IsValid(PropertyPanelWidgetClass)) {
		PropertyPanelWidget = CreateWidget<UPropertyPanelWidget>(GetWorld(), PropertyPanelWidgetClass);
		PropertyPanelWidget->PropertyWidgetSwitcher->SetActiveWidgetIndex(2);
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

	FVector Dimensions{ abs(XDistance) + (2 * EdgeOffset), abs(YDistance) + (2 * EdgeOffset), ZDistance };
	FVector Offset{ abs(XDistance) / 2 , abs(YDistance) / 2, ZDistance / 2};
	//FVector NewStartPoint{ StartPoint };

	if (XDistance >= 0.0 && YDistance >= 0.0) {
		FloorMeshComponent->SetWorldRotation(FRotator{ 0.0 });
	}
	else if (XDistance >= 0.0 && YDistance < 0.0) {
		FloorMeshComponent->SetWorldRotation(FRotator{ 0.0,0.0,180.0 });
		Offset.Z *= -1.0;
	}
	else if (XDistance < 0.0 && YDistance >= 0.0) {
		FloorMeshComponent->SetWorldRotation(FRotator{ 180.0,0.0,0.0 });
		Offset.Z *= -1.0;
	}
	else {
		FloorMeshComponent->SetWorldRotation(FRotator{ 180.0,0.0, 180.0 });
	}

	//if (XDistance >= 0.0) {
	//	NewStartPoint.X -= EdgeOffset;
	//}
	//else {
	//	NewStartPoint.X += EdgeOffset;
	//}
	//if (YDistance >= 0.0) {
	//	NewStartPoint.Y -= EdgeOffset;
	//}
	//else {
	//	NewStartPoint.Y += EdgeOffset;
	//}

	//SetActorLocation(NewStartPoint);

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

void AFloorActor::UpdateSpinBoxValue() {
	double XDistance = EndPoint.X - StartPoint.X;
	double YDistance = EndPoint.Y - StartPoint.Y;
	double ZDistance = 2.0;

	if (IsValid(PropertyPanelWidget)) {
		PropertyPanelWidget->FloorLengthSpinbox->SetValue(abs(XDistance));
		PropertyPanelWidget->FloorWidthSpinbox->SetValue(abs(YDistance));
		PropertyPanelWidget->FloorHeightSpinbox->SetValue(ZDistance);
	}
}
