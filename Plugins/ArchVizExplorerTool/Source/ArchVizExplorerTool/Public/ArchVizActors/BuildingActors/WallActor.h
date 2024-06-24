// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArchVizActors/BuildingActor.h"
#include "WallActor.generated.h"

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

	void GenerateWallSegments(double Length = 0.0);
	void DestroyWallSegments();

	void HandlePreviewState();
	void HandleGeneratingState();
	void HandleMovingState();

	void HandleEdgeOffset();
};