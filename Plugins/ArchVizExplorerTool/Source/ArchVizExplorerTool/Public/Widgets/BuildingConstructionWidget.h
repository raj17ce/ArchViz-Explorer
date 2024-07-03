// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums/BuildingSubModeEnum.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/Button.h"
#include "BuildingConstructionWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnBuildingSubModeChange, EBuildingSubMode)

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORERTOOL_API UBuildingConstructionWidget : public UUserWidget {
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	FOnBuildingSubModeChange OnBuildingSubModeChange;

	UFUNCTION()
	void HandleWallSubModeButtonClick();

	UFUNCTION()
	void HandleDoorSubModeButtonClick();

	UFUNCTION()
	void HandleFloorSubModeButtonClick();

	UFUNCTION()
	void HandleRoofSubModeButtonClick();

	UFUNCTION()
	void HighlightSelectedColour(EBuildingSubMode NewBuildingSubMode);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CurrentModeText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* WallSubModeButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* DoorSubModeButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* FloorSubModeButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* RoofSubModeButton;

private:
	void SetNormalTint(FButtonStyle& ButtonStyle);
	void SetHighlightedTint(FButtonStyle& ButtonStyle);
};
