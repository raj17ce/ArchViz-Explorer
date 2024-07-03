// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/Border.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "DataAssets/InteriorDataAsset.h"
#include "InteriorItemWidget.h"
#include "InteriorScrollBox.generated.h"

DECLARE_DELEGATE_OneParam(FOnItemSelected, FInteriorAssetData)

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORERTOOL_API UInteriorScrollBox : public UUserWidget {
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	void PopulateWidget(UInteriorDataAsset* DataAsset);

	FOnItemSelected OnItemSelected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UInteriorItemWidget> InteriorItemWidgetClass;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UBorder* BorderBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* ScrollBox;

	UFUNCTION()
	void HandleItemButtonClicked(FInteriorAssetData Asset);
};
