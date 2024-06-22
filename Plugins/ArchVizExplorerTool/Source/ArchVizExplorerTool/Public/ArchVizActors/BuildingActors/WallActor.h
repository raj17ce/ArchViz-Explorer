// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArchVizActors/BuildingActor.h"
#include "WallActor.generated.h"

UENUM(BlueprintType)
enum class EWallActorState : uint8 {
	Selected,
	Preview,
	Generating,
	Moving
};

UCLASS()
class ARCHVIZEXPLORERTOOL_API AWallActor : public ABuildingActor {
	GENERATED_BODY()

public:
	friend class UWallSubMode;

	// Sets default values for this actor's properties
	AWallActor();

	void SetStartLocation(const FVector& NewStartLocation);
	const FVector& GetStartLocation() const;

	void SetEndLocation(const FVector& NewEndLocation);
	const FVector& GetEndLocation() const;

	void SetState(EWallActorState NewState);
	EWallActorState GetState() const;

	UPROPERTY()
	TArray<UStaticMeshComponent*> WallSegments;

	UPROPERTY()
	USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wall")
	UStaticMesh* WallStaticMesh;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	FVector StartLocation;
	FVector EndLocation;

	EWallActorState State;

	void GenerateWallSegments(double Length = 0.0);
	void DestroyWallSegments();
};