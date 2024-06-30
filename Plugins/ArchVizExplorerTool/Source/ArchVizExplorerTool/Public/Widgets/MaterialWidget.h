// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MaterialScrollBox.h"
#include "DataAssets/MaterialDataAsset.h"
#include "MaterialWidget.generated.h"

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORERTOOL_API UMaterialWidget : public UUserWidget {
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	//ScrollBox
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UMaterialScrollBox* MaterialScrollBox;

	//Data-Assets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data Asset")
	UMaterialDataAsset* RoadMaterialDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data Asset")
	UMaterialDataAsset* WallMaterialDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data Asset")
	UMaterialDataAsset* FloorMaterialDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data Asset")
	UMaterialDataAsset* RoofMaterialDataAsset;
};
