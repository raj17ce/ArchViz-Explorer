// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums/ArchVizModeEnum.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/Border.h"
#include "UMG/Public/Components/HorizontalBox.h"
#include "UMG/Public/Components/Button.h"
#include "ArchVizModeWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnArchVizModeChange, EArchVizMode)

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORERTOOL_API UArchVizModeWidget : public UUserWidget {
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	FOnArchVizModeChange OnArchVizModeChange;

	UFUNCTION()
	void HandleRoadModeButtonClick();

	UFUNCTION()
	void HandleBuildingModeButtonClick();

	UFUNCTION()
	void HandleInteriorModeButtonClick();

	UFUNCTION()
	void HandleSaveGameModeButtonClick();

	UFUNCTION()
	void HighlightSelectedColour(EArchVizMode NewArchVizMode);

	UFUNCTION()
	void RemoveHighlights();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* SaveGameModeButton;

private:
	void SetNormalTint(FButtonStyle& ButtonStyle);
	void SetHighlightedTint(FButtonStyle& ButtonStyle);
};
