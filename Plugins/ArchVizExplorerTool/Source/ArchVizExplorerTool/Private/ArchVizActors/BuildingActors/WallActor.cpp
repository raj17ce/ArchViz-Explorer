// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizActors/BuildingActors/WallActor.h"

// Sets default values
AWallActor::AWallActor() : WallSegmentIndex{ -1 }, WallSegmentRotation{ 0.0 }, PreviewWallSegment{ nullptr } {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComponent);
}

// Called when the game starts or when spawned
void AWallActor::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void AWallActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

int32 AWallActor::GetSegmentIndex() const {
	return WallSegmentIndex;
}

void AWallActor::SetSegmentIndex(int32 Index) {
	WallSegmentIndex = Index;
}

FRotator AWallActor::GetSegmentRotation() const {
	return WallSegmentRotation;
}

void AWallActor::SetSegmentRotation(const FRotator& Rotation) {
	WallSegmentRotation = Rotation;
}