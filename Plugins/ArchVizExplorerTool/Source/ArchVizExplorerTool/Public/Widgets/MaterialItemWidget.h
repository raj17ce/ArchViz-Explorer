// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataAssets/MaterialDataAsset.h"
#include "UMG/Public/Components/Border.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/Image.h"
#include "MaterialItemWidget.generated.h"

DECLARE_DELEGATE_OneParam(FOnMaterialItemButtonClicked, FMaterialAssetData)

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORERTOOL_API UMaterialItemWidget : public UUserWidget {
	GENERATED_BODY()

public:
	void SetItemData(const FMaterialAssetData& NewItemData);

	FOnMaterialItemButtonClicked OnItemButtonClicked;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UBorder* ItemBorderBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* ItemButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ItemImage;

	virtual void NativeOnInitialized() override;
private:
	FMaterialAssetData ItemData;

	UFUNCTION()
	void HandleItemButtonClick();
};
