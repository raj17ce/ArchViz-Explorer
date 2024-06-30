// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ArchVizActors/BuildingActor.h"
#include "RoofActor.generated.h"

UCLASS()
class ARCHVIZEXPLORERTOOL_API ARoofActor : public ABuildingActor {
	GENERATED_BODY()

public:
	friend class URoofSubMode;

	// Sets default values for this actor's properties
	ARoofActor();

	void SetStartPoint(const FVector& NewStartPoint);
	const FVector& GetStartPoint();

	void SetEndPoint(const FVector& NewEndPoint);
	const FVector& GetEndPoint();

	UPROPERTY()
	USceneComponent* SceneComponent;

	UPROPERTY()
	UProceduralMeshComponent* RoofMeshComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FVector StartPoint;
	FVector EndPoint;

	void HandlePreviewState();
	void HandleGeneratingState();
	void HandleMovingState();

	void GenerateRoof(const FVector& Dimensions, const FVector& Offset);
	void DestroyRoof();

	void UpdateSpinBoxValue();

	UFUNCTION()
	void HandleMaterialChange(FMaterialAssetData MaterialData);
};
