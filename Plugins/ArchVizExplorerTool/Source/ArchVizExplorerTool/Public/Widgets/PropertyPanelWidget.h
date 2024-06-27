// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/SpinBox.h"
#include "UMG/Public/Components/Button.h"
#include "PropertyPanelWidget.generated.h"

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORERTOOL_API UPropertyPanelWidget : public UUserWidget {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UWidgetSwitcher* PropertyWidgetSwitcher;

	//Wall
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USpinBox* WallLengthSpinbox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* WallCloseButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* WallNewButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* WallDeleteButton;

	//Door
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* DoorCloseButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* DoorNewButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* DoorDeleteButton;

	//Floor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USpinBox* FloorLengthSpinbox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USpinBox* FloorWidthSpinbox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USpinBox* FloorHeightSpinbox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* FloorCloseButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* FloorNewButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* FloorDeleteButton;

	//Roof
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USpinBox* RoofLengthSpinbox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USpinBox* RoofWidthSpinbox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USpinBox* RoofHeightSpinbox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* RoofCloseButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* RoofNewButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* RoofDeleteButton;
};
