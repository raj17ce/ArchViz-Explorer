// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArchVizActors/BuildingActor.h"
#include "DataAssets/MaterialDataAsset.h"
#include "WallActor.generated.h"

class ADoorActor;

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

	float GetLength() const;
	void SetLength(float NewLength);

	TArray<UStaticMeshComponent*> GetWallSegments() const;
	void SetWallSegments(TArray<UStaticMeshComponent*> NewWallSegments);

	void AttachDoorComponent(UPrimitiveComponent* Component, ADoorActor* DoorActor);
	void DetachDoorComponent(ADoorActor* DoorActor);
	void DestroyDoorComponents();

	void GenerateWallSegments();
	void UpdateDoorSegments();

	void UpdateLengthSpinBoxValue();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	FVector StartLocation;
	FVector EndLocation;

	TMap<int32, ADoorActor*> DoorMapping;

	UPROPERTY()
	TArray<UStaticMeshComponent*> WallSegments;

	UPROPERTY()
	USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Wall")
	UStaticMesh* WallStaticMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Wall")
	UStaticMesh* DoorWallStaticMesh;

	float Length;

	void DestroyWallSegments();

	void HandlePreviewState();
	void HandleGeneratingState();
	void HandleMovingState();

	void HandleEdgeOffset();

	UFUNCTION()
	void HandleMaterialChange(FMaterialAssetData MaterialData);

	virtual void ApplyMaterial() override;
	virtual void ApplyPreviewMaterial() override;
};