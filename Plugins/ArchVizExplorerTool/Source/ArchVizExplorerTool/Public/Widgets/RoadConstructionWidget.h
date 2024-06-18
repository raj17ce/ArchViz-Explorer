// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/TextBlock.h"
#include "RoadConstructionWidget.generated.h"

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORERTOOL_API URoadConstructionWidget : public UUserWidget {
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CurrentModeText;
};