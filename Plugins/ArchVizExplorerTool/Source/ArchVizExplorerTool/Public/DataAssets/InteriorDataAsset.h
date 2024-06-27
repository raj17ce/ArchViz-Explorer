// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InteriorDataAsset.generated.h"

USTRUCT(Blueprintable)
struct FInteriorAssetData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh;
};

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORERTOOL_API UInteriorDataAsset : public UDataAsset {
	GENERATED_BODY()

public:
	EInteriorAssetType InteriorAssetType;

	TArray<FInteriorAssetData> InteriorAssets;
};
