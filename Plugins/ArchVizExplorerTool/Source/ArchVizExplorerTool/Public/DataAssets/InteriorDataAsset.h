// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Enums/InteriorAssetTypeEnum.h"
#include "InteriorDataAsset.generated.h"

USTRUCT(Blueprintable)
struct FInteriorAssetData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInteriorAssetType InteriorAssetType;
};

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORERTOOL_API UInteriorDataAsset : public UDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FInteriorAssetData> InteriorAssets;
};
