// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Materials/MaterialInterface.h"
#include "Engine/Texture2D.h"
#include "MaterialDataAsset.generated.h"

USTRUCT(Blueprintable)
struct FMaterialAssetData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* Material;
};

/**
 * 
 */
UCLASS()
class ARCHVIZEXPLORERTOOL_API UMaterialDataAsset : public UDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FMaterialAssetData> MaterialAssets;
};
