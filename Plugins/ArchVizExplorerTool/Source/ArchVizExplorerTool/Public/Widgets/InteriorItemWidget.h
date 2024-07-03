// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/Border.h"
#include "UMG/Public/Components/Button.h"
#include "UMG/Public/Components/Image.h"
#include "DataAssets/InteriorDataAsset.h"
#include "InteriorItemWidget.generated.h"

DECLARE_DELEGATE_OneParam(FOnItemButtonClicked, FInteriorAssetData)

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORERTOOL_API UInteriorItemWidget : public UUserWidget {
	GENERATED_BODY()

public:
	void SetItemData(const FInteriorAssetData& NewItemData);

	FOnItemButtonClicked OnItemButtonClicked;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UBorder* ItemBorderBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* ItemButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ItemImage;

	virtual void NativeOnInitialized() override;
private:
	FInteriorAssetData ItemData;

	UFUNCTION()
	void HandleItemButtonClick();
};
