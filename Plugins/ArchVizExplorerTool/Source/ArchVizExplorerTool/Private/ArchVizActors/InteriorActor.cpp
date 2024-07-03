// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizActors/InteriorActor.h"
#include "ArchVizActors/BuildingActors/WallActor.h"

// Sets default values
AInteriorActor::AInteriorActor() : State{ EInteriorActorState::None } {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComponent);

	InteriorComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Interior Component"));
	InteriorComponent->SetupAttachment(SceneComponent);
}

void AInteriorActor::ShowWidget() {
	if (IsValid(PropertyPanelWidget)) {
		PropertyPanelWidget->AddToViewport();
	}
}

void AInteriorActor::HideWidget() {
	if (IsValid(PropertyPanelWidget)) {
		PropertyPanelWidget->RemoveFromParent();
	}
}

void AInteriorActor::SetState(EInteriorActorState NewState) {
	State = NewState;

	HandleStateChange();
}

EInteriorActorState AInteriorActor::GetState() const {
	return State;
}

void AInteriorActor::HandleStateChange() {
	if (State == EInteriorActorState::Selected) {
		ShowWidget();
		HighlightSelectedActor();
	}
	else {
		HideWidget();
		UnhighlightDeselectedActor();
	}
}

void AInteriorActor::SetActorAssetData(const FInteriorAssetData& NewAssetData) {
	AssetData = NewAssetData;
	if (IsValid(InteriorComponent) && IsValid(AssetData.StaticMesh)) {
		InteriorComponent->SetStaticMesh(AssetData.StaticMesh);
	}
}

const FInteriorAssetData& AInteriorActor::GetActorAssetData() const {
	return AssetData;
}

void AInteriorActor::AdjustWallInteriorRotation(FRotator& Rotation) {
	if (Rotation.Yaw >= 85.0 && Rotation.Yaw <= 95.0) {
		Rotation.Yaw = -90.0;
	}
	else if (Rotation.Yaw >= -5.0 && Rotation.Yaw <= 5.0) {
		Rotation.Yaw = 180.0;
	}
	else if (Rotation.Yaw >= 175 && Rotation.Yaw <= 185) {
		Rotation.Yaw = 0.0;
	}
	else {
		Rotation.Yaw = 90.0;
	}
}

// Called when the game starts or when spawned
void AInteriorActor::BeginPlay() {
	Super::BeginPlay();

	if (IsValid(PropertyPanelWidgetClass)) {
		PropertyPanelWidget = CreateWidget<UPropertyPanelWidget>(GetWorld(), PropertyPanelWidgetClass);
		PropertyPanelWidget->PropertyWidgetSwitcher->SetActiveWidgetIndex(4);
	}
}

// Called every frame
void AInteriorActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (State == EInteriorActorState::Preview) {
		HandlePreviewState();
	}
	else if (State == EInteriorActorState::Moving) {
		HandleMovingState();
	}
}

void AInteriorActor::HandlePreviewState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});

	if (IsValid(HitResult.GetActor())) {
		if (AssetData.InteriorAssetType == EInteriorAssetType::WallPlaceable && HitResult.GetActor()->IsA(AWallActor::StaticClass())) {
			FRotator ActorRotation = HitResult.GetActor()->GetActorRotation();
			AdjustWallInteriorRotation(ActorRotation);
			SetActorRotation(ActorRotation);
		}
		SetActorLocation(HitResult.Location);
	}
}

void AInteriorActor::HandleMovingState() {
	FHitResult HitResult = GetHitResult(TArray<AActor*>{this});

	if (IsValid(HitResult.GetActor())) {
		if (AssetData.InteriorAssetType == EInteriorAssetType::WallPlaceable && HitResult.GetActor()->IsA(AWallActor::StaticClass())) {
			FRotator ActorRotation = HitResult.GetActor()->GetActorRotation();
			AdjustWallInteriorRotation(ActorRotation);
			SetActorRotation(ActorRotation);
		}
		SetActorLocation(HitResult.Location);
	}
}

