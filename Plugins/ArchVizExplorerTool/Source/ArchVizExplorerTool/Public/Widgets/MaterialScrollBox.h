// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataAssets/MaterialDataAsset.h"
#include "MaterialItemWidget.h"
#include "UMG/Public/Components/Border.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "MaterialScrollBox.generated.h"

DECLARE_DELEGATE_OneParam(FOnMaterialItemSelected, FMaterialAssetData)

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORERTOOL_API UMaterialScrollBox : public UUserWidget {
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	void PopulateWidget(UMaterialDataAsset* DataAsset);

	FOnMaterialItemSelected OnItemSelected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UMaterialItemWidget> MaterialItemWidgetClass;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UBorder* BorderBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* ScrollBox;

	UFUNCTION()
	void HandleItemButtonClicked(FMaterialAssetData Asset);
};
