// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ArchVizActors/BuildingActor.h"
#include "FloorActor.generated.h"

UCLASS()
class ARCHVIZEXPLORERTOOL_API AFloorActor : public ABuildingActor {
	GENERATED_BODY()

public:
	friend class UFloorSubMode;

	// Sets default values for this actor's properties
	AFloorActor();

	void SetStartPoint(const FVector& NewStartPoint);
	const FVector& GetStartPoint() const;

	void SetEndPoint(const FVector& NewEndPoint);
	const FVector& GetEndPoint() const;

	const FVector& GetDimensions() const;
	void SetDimensions(const FVector& NewDimensions);

	const FVector& GetOffset() const;
	void SetOffset(const FVector& NewOffset);

	void GenerateFloor();
	void UpdateSpinBoxValue();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FVector StartPoint;
	FVector EndPoint;

	UPROPERTY()
	USceneComponent* SceneComponent;

	UPROPERTY()
	UProceduralMeshComponent* FloorMeshComponent;

	FVector Dimensions;
	FVector Offset;

	void HandlePreviewState();
	void HandleGeneratingState();
	void HandleMovingState();

	void DestroyFloor();

	UFUNCTION()
	void HandleMaterialChange(FMaterialAssetData MaterialData);
};
