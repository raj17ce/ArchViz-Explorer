// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ArchVizActors/RoadActor.h"
#include "Enums/InteriorAssetTypeEnum.h"
#include "Materials/MaterialInterface.h"
#include "Engine/StaticMesh.h"
#include "DataAssets/InteriorDataAsset.h"
#include "ArchVizSaveGame.generated.h"

USTRUCT()
struct FRoadData {
	GENERATED_BODY()

	UPROPERTY()
	int32 ID;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	TArray<FVector> SplinePoints;

	UPROPERTY()
	ERoadType RoadType;

	UPROPERTY()
	float Width;

	UPROPERTY()
	UMaterialInterface* Material;

	UPROPERTY()
	int32 ParentActorID;
};

USTRUCT()
struct FWallData {

	GENERATED_BODY()

	UPROPERTY()
	int32 ID;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	double Length;

	UPROPERTY()
	UMaterialInterface* Material;

	UPROPERTY()
	int32 ParentActorID;
};

USTRUCT()
struct FFloorData {
	GENERATED_BODY()

	UPROPERTY()
	int32 ID;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	FVector StartPoint;

	UPROPERTY()
	FVector EndPoint;

	UPROPERTY()
	FVector Dimensions;

	UPROPERTY()
	FVector Offset;

	UPROPERTY()
	UMaterialInterface* Material;

	UPROPERTY()
	int32 ParentActorID;
};

USTRUCT()
struct FRoofData {
	GENERATED_BODY()

	UPROPERTY()
	int32 ID;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	FVector StartPoint;

	UPROPERTY()
	FVector EndPoint;

	UPROPERTY()
	FVector Dimensions;

	UPROPERTY()
	FVector Offset;

	UPROPERTY()
	UMaterialInterface* Material;

	UPROPERTY()
	int32 ParentActorID;
};


USTRUCT()
struct FDoorData {
	GENERATED_BODY()

	UPROPERTY()
	int32 ID;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	bool bIsOpen;

	UPROPERTY()
	int32 ParentActorID;

	UPROPERTY()
	int32 ParentComponentIndex;
};

USTRUCT()
struct FInteriorData {
	GENERATED_BODY()

	UPROPERTY()
	int32 ID;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	FInteriorAssetData AssetData;

	UPROPERTY()
	EInteriorAssetType InteriorType;

	UPROPERTY()
	int32 ParentActorID;
};

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORERTOOL_API UArchVizSaveGame : public USaveGame {
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FRoadData> RoadActors;

	UPROPERTY()
	TArray<FWallData> WallActors;

	UPROPERTY()
	TArray<FFloorData> FloorActors;

	UPROPERTY()
	TArray<FRoofData> RoofActors;

	UPROPERTY()
	TArray<FDoorData> DoorActors;

	UPROPERTY()
	TArray<FInteriorData> InteriorActors;
};
