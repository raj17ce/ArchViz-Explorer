// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizActors/BuildingActors/FloorActor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMeshGenerator.h"

// Sets default values
AFloorActor::AFloorActor() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));
	SetRootComponent(ProceduralMeshComponent);

	ProceduralMeshGenerator::GenerateCube(ProceduralMeshComponent, 0, FVector{200, 20, 300}, FVector{100,0,150});
}

// Called when the game starts or when spawned
void AFloorActor::BeginPlay() {
	Super::BeginPlay();

}

// Called every frame
void AFloorActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

