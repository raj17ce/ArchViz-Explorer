// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizActors/InteriorActor.h"

// Sets default values
AInteriorActor::AInteriorActor() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AInteriorActor::SetState(EInteriorActorState NewInteriorActorState) {
	InteriorActorState = NewInteriorActorState;
}

EInteriorActorState AInteriorActor::GetState() const {
	return InteriorActorState;
}

void AInteriorActor::SetInteriorAssetType(EInteriorAssetType NewInteriorAssetType) {
	InteriorAssetType = NewInteriorAssetType;
}

EInteriorAssetType AInteriorActor::GetInteriorAssetType() const {
	return InteriorAssetType;
}

// Called when the game starts or when spawned
void AInteriorActor::BeginPlay() {
	Super::BeginPlay();

}

// Called every frame
void AInteriorActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

