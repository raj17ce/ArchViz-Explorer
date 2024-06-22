// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizActors/ArchVizActor.h"

// Sets default values
AArchVizActor::AArchVizActor() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AArchVizActor::BeginPlay() {
	Super::BeginPlay();

}

// Called every frame
void AArchVizActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

FHitResult AArchVizActor::GetHitResult(const TArray<AActor*>& ActorsToIgnore) const {
	FHitResult HitResult{};

	FVector WorldLocation{}, WorldDirection{};

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection)) {
		FVector TraceStart = WorldLocation;
		FVector TraceEnd = WorldLocation + (WorldDirection * 10000.0);

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.bTraceComplex = true;
		CollisionQueryParams.AddIgnoredActor(PlayerController->GetPawn());

		for (const auto& Actor : ActorsToIgnore) {
			CollisionQueryParams.AddIgnoredActor(Actor);
		}

		GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionQueryParams);
	}

	return HitResult;
}