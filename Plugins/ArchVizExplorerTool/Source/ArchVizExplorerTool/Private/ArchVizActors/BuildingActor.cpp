// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizActors/BuildingActor.h"

// Sets default values
ABuildingActor::ABuildingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABuildingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuildingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuildingActor::RotateActor(double Degree) {
	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Yaw = static_cast<int32>(CurrentRotation.Yaw + Degree) % 360;
	
	SetActorRotation(CurrentRotation);
}

