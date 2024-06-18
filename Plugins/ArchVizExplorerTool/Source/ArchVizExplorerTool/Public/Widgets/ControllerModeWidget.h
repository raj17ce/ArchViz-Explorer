// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ControllerModeEnum.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/Border.h"
#include "UMG/Public/Components/HorizontalBox.h"
#include "UMG/Public/Components/Button.h"
#include "ControllerModeWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnControllerModeChange, EControllerMode)

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORERTOOL_API UControllerModeWidget : public UUserWidget {
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	FOnControllerModeChange OnControllerModeChange;

	UFUNCTION()
	void HandleRoadModeButtonClick();

	UFUNCTION()
	void HandleBuildingModeButtonClick();

	UFUNCTION()
	void HandleInteriorModeButtonClick();

	UFUNCTION()
	void HighlightSelectedColour(EControllerMode NewControllerMode);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UBorder* ModeBorderBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* ModeHorizontalBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* RoadModeButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* BuildingModeButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* InteriorModeButton;

private:
	void SetNormalTint(FButtonStyle& ButtonStyle);
	void SetHighlightedTint(FButtonStyle& ButtonStyle);
};
