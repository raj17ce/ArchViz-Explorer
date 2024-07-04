// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizActors/BuildingActors/WallActor.h"
#include "ArchVizUtility.h"
#include "ArchVizActors/BuildingActors/DoorActor.h"
#include "ArchVizController.h"

// Sets default values
AWallActor::AWallActor() : WallStaticMesh{nullptr}, DoorWallStaticMesh{nullptr}, Length{0.0f} {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComponent);
	SceneComponent->SetMobility(EComponentMobility::Movable);

	//PrimaryActorTick.TickInterval = 0.1;
}

void AWallActor::SetStartLocation(const FVector& NewStartLocation) {
	StartLocation = NewStartLocation;
}

const FVector& AWallActor::GetStartLocation() const {
	return StartLocation;
}

void AWallActor::SetEndLocation(const FVector& NewEndLocation) {
	EndLocation = NewEndLocation;
}

const FVector& AWallActor::GetEndLocation() const {
	return EndLocation;
}

float AWallActor::GetLength() const {
	return Length;
}

void AWallActor::SetLength(float NewLength) {
	Length = NewLength;
}

TArray<UStaticMeshComponent*> AWallActor::GetWallSegments() const {
	return WallSegments;
}

void AWallActor::SetWallSegments(TArray<UStaticMeshComponent*> NewWallSegments) {
	WallSegments = NewWallSegments;
}

void AWallActor::AttachDoorComponent(UPrimitiveComponent* Component, ADoorActor* DoorActor) {
	if (auto* DoorComponent = Cast<UStaticMeshComponent>(Component)) {
		int32 SegmentIndex = WallSegments.Find(DoorComponent);

		if (SegmentIndex != INDEX_NONE) {
			WallSegments[SegmentIndex]->SetStaticMesh(DoorWallStaticMesh);
			DoorMapping.Add(SegmentIndex, DoorActor);

			DoorActor->AttachToComponent(WallSegments[SegmentIndex], FAttachmentTransformRules::KeepRelativeTransform, TEXT("DoorSocket"));
			DoorActor->SetActorRelativeRotation(FRotator::ZeroRotator);
			DoorActor->SetActorRelativeLocation(FVector::ZeroVector);

			DoorActor->SetParentWallComponentIndex(SegmentIndex);
		}
	}
}

void AWallActor::DetachDoorComponent(ADoorActor* DoorActor) {
	int32 SegmentIndex{-1};

	if (DoorMapping.FindKey(DoorActor)) {
		SegmentIndex = *(DoorMapping.FindKey(DoorActor));
	}

	if (SegmentIndex != -1) {
		DoorActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		WallSegments[SegmentIndex]->SetStaticMesh(WallStaticMesh);
	}

	DoorMapping.Remove(SegmentIndex);
}

void AWallActor::DestroyDoorComponents() {
	for (auto& [SegmentIndex, DoorActor] : DoorMapping) {
		if (IsValid(DoorActor)) {
			DoorActor->Destroy();
		}
	}

	DoorMapping.Empty();
}

// Called when the game starts or when spawned
void AWallActor::BeginPlay() {
	Super::BeginPlay();

	if (IsValid(PropertyPanelWidgetClass)) {
		PropertyPanelWidget = CreateWidget<UPropertyPanelWidget>(GetWorld(), PropertyPanelWidgetClass);
		PropertyPanelWidget->PropertyWidgetSwitcher->SetActiveWidgetIndex(0);
	}
	if (IsValid(MaterialWidgetClass)) {
		MaterialWidget = CreateWidget<UMaterialWidget>(GetWorld(), MaterialWidgetClass);
		if (IsValid(MaterialWidget->MaterialScrollBox)) {
			MaterialWidget->MaterialScrollBox->PopulateWidget(MaterialWidget->WallMaterialDataAsset);
			MaterialWidget->MaterialScrollBox->OnItemSelected.BindUObject(this, &AWallActor::HandleMaterialChange);
		}
	}
}

// Called every frame
void AWallActor::Tick(float DeltaTime) {
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

void AWallActor::GenerateWallSegments() {
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
		WallMeshComponent->RegisterComponent();
		WallMeshComponent->SetMobility(EComponentMobility::Movable);

		if (IsValid(WallStaticMesh)) {
			WallMeshComponent->SetStaticMesh(WallStaticMesh);
			WallSegments.Add(WallMeshComponent);
		}

		WallMeshComponent->SetRelativeLocation(FVector{SegmentIndex * WallSize.X, 0, 0});
	}

	ApplyPreviewMaterial();
	UpdateDoorSegments();
}

void AWallActor::UpdateDoorSegments() {
	int32 NumberOfSegments = WallSegments.Num();

	for (auto It = DoorMapping.CreateIterator(); It; ++It) {
		int32 SegmentIndex = It.Key();
		ADoorActor* DoorActor = It.Value();

		if (SegmentIndex >= NumberOfSegments) {
			if (IsValid(DoorActor)) {
				DoorActor->Destroy();
			}
			It.RemoveCurrent();
			continue;
		}

		if (WallSegments.IsValidIndex(SegmentIndex)) {
			WallSegments[SegmentIndex]->SetStaticMesh(DoorWallStaticMesh);

			DoorActor->AttachToComponent(WallSegments[SegmentIndex], FAttachmentTransformRules::KeepRelativeTransform, TEXT("DoorSocket"));
			DoorActor->SetActorRelativeRotation(FRotator::ZeroRotator);
			DoorActor->SetActorRelativeLocation(FVector::ZeroVector);
		}
	}
}

void AWallActor::DestroyWallSegments() {
	for (auto& Segment : WallSegments) {
		Segment->DestroyComponent();
		Segment = nullptr;
	}

	WallSegments.Empty();
}

void AWallActor::HandlePreviewState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

	SetActorLocation(HitResult.Location);
}

void AWallActor::HandleGeneratingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

	if (EndLocation == HitResult.Location) {
		return;
	}

	SetEndLocation(HitResult.Location);

	double XDistance = EndLocation.X - StartLocation.X;
	double YDistance = EndLocation.Y - StartLocation.Y;

	if (EndLocation != StartLocation) {
		if (abs(XDistance) >= abs(YDistance)) {
			Length = abs(XDistance);

			if (XDistance >= 0) {
				SetActorRotation(FRotator{ 0.0 });
			}
			else {
				SetActorRotation(FRotator{ 0.0, 180.0, 0.0 });
			}
		}
		else {
			Length = abs(YDistance);

			if (YDistance >= 0) {
				SetActorRotation(FRotator{ 0.0, 90.0, 0.0 });
			}
			else {
				SetActorRotation(FRotator{ 0.0, -90, 0.0 });
			}
		}

		GenerateWallSegments();
		HandleEdgeOffset();
	}
}

void AWallActor::HandleMovingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});
	HitResult.Location = ArchVizUtility::GetSnappedLocation(HitResult.Location);

	SetActorLocation(HitResult.Location);
	SetStartLocation(HitResult.Location);
	HandleEdgeOffset();
}

void AWallActor::HandleEdgeOffset() {
	FRotator ActorRotation = GetActorRotation();

	double Offset{}; 

	if (IsValid(WallStaticMesh)) {
		Offset = (WallStaticMesh->GetBoundingBox().GetSize().Y / 2);
	}

	if (ActorRotation.Yaw >= -5 && ActorRotation.Yaw <= 5) {
		SetActorLocation(FVector{ StartLocation.X + Offset, StartLocation.Y, StartLocation.Z });
	}
	else if (ActorRotation.Yaw >= 85 && ActorRotation.Yaw <= 95) {
		SetActorLocation(FVector{ StartLocation.X, StartLocation.Y + Offset, StartLocation.Z });
	}
	else if (ActorRotation.Yaw >= 175 && ActorRotation.Yaw <= 185) {
		SetActorLocation(FVector{ StartLocation.X - Offset, StartLocation.Y, StartLocation.Z });
	}
	else if (ActorRotation.Yaw >= -95 && ActorRotation.Yaw <= -85) {
		SetActorLocation(FVector{ StartLocation.X, StartLocation.Y - Offset, StartLocation.Z });
	}
}

void AWallActor::UpdateLengthSpinBoxValue() {
	if (IsValid(PropertyPanelWidget)) {
		PropertyPanelWidget->WallLengthSpinbox->SetValue(Length);
	}
}

void AWallActor::HandleMaterialChange(FMaterialAssetData MaterialData) {
	if (MaterialData.Material) {
		Material = MaterialData.Material;
		ApplyMaterial();
		if (auto* ArchVizController = Cast<AArchVizController>(GetWorld()->GetFirstPlayerController())) {
			ArchVizController->AddSuccessMessage(FText::FromString("Material Applied Successfully"), 1.5f);
		}
	}
}

void AWallActor::ApplyMaterial() {
	if (IsValid(Material)) {
		if (auto* Dynamicmaterial = UMaterialInstanceDynamic::Create(Material, this)) {
			Dynamicmaterial->SetVectorParameterValue(FName("Tiling/Offset"), FLinearColor(0.33, 0.33, 0, 0));
			
			for (auto Segment : WallSegments) {
				if (IsValid(Segment)) {
					Segment->SetMaterial(0, Dynamicmaterial);
				}
			}
		}
	}
}

void AWallActor::ApplyPreviewMaterial() {
	if (IsValid(PreviewMaterial)) {
		if (auto* Dynamicmaterial = UMaterialInstanceDynamic::Create(PreviewMaterial, this)) {
			Dynamicmaterial->SetVectorParameterValue(FName("Tiling/Offset"), FLinearColor(0.33, 0.33, 0, 0));

			for (auto Segment : WallSegments) {
				if (IsValid(Segment)) {
					Segment->SetMaterial(0, Dynamicmaterial);
				}
			}
		}
	}
}
