// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/Button.h"
#include "DataAssets/InteriorDataAsset.h"
#include "InteriorScrollBox.h"
#include "InteriorDesignWidget.generated.h"

DECLARE_DELEGATE(FOnInteriorTypeChange)

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORERTOOL_API UInteriorDesignWidget : public UUserWidget {
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;

	//ScrollBox
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UInteriorScrollBox* InteriorScrollBox;
	 
	FOnInteriorTypeChange OnInteriorTypeChange;

	void ShowScrollBox();
	void HideScrollBox();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CurrentModeText;

	//Buttons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* ChairButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* TableButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* BedButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* CeilingLightButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* LampButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* FrameButton;

	//Data-Assets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data Asset")
	UInteriorDataAsset* ChairDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data Asset")
	UInteriorDataAsset* TableDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data Asset")
	UInteriorDataAsset* BedDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data Asset")
	UInteriorDataAsset* CeilingLightDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data Asset")
	UInteriorDataAsset* LampDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data Asset")
	UInteriorDataAsset* FrameDataAsset;

private:
	void HandleButtonClick(UInteriorDataAsset* DataAsset);

	UFUNCTION()
	void HandleChairButtonClick();

	UFUNCTION()
	void HandleTableButtonClick();

	UFUNCTION()
	void HandleBedButtonClick();

	UFUNCTION()
	void HandleCeilingLightButtonClick();

	UFUNCTION()
	void HandleLampButtonClick();

	UFUNCTION()
	void HandleFrameButtonClick();
};
