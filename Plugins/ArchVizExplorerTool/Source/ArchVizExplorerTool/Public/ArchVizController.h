// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "RoadActor.h"
#include "WallActor.h"
#include "Enums/ArchVizModeEnum.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "Widgets/ArchVizModeWidget.h"
#include "Widgets/RoadConstructionWidget.h"
#include "Widgets/BuildingConstructionWidget.h"
#include "Widgets/InteriorDesignWidget.h"
#include "ArchVizModes/ArchVizMode.h"
#include "ArchVizModes/RoadConstructionMode.h"
#include "ArchVizModes/BuildingConstructionMode.h"
#include "ArchVizModes/InteriorDesignMode.h"
#include "ArchVizController.generated.h"

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORERTOOL_API AArchVizController : public APlayerController {
	GENERATED_BODY()

public:
	AArchVizController();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	UFUNCTION()
	void HandleArchVizModeChange(EArchVizMode NewArchVizMode);

private:
	FInputModeGameAndUI InputModeGameAndUI{};

	EArchVizMode CurrentArchVizMode;
	IArchVizMode* CurrentArchVizModePtr;

	//Modes
	UPROPERTY(EditDefaultsOnly, Category="Mode")
	TSubclassOf<URoadConstructionMode> RoadConstructionModeClass;

	UPROPERTY(VisibleDefaultsOnly, Category="Mode")
	URoadConstructionMode* RoadConstructionMode;

	UPROPERTY(EditDefaultsOnly, Category="Mode")
	TSubclassOf<UBuildingConstructionMode> BuildingConstructionModeClass;

	UPROPERTY(VisibleDefaultsOnly, Category="Mode")
	UBuildingConstructionMode* BuildingConstructionMode;

	UPROPERTY(EditDefaultsOnly, Category="Mode")
	TSubclassOf<UInteriorDesignMode> InteriorDesignModeClass;

	UPROPERTY(VisibleDefaultsOnly, Category="Mode")
	UInteriorDesignMode* InteriorDesignMode;

	//Widgets
	UPROPERTY(EditDefaultsOnly, Category="Widget")
	TSubclassOf<UArchVizModeWidget> ArchVizModeWidgetClass;

	UPROPERTY(VisibleDefaultsOnly, Category="Widget")
	UArchVizModeWidget* ArchVizModeWidget;

	UPROPERTY(EditDefaultsOnly, Category="Widget")
	TSubclassOf<URoadConstructionWidget> RoadWidgetClass;

	UPROPERTY(VisibleDefaultsOnly, Category="Widget")
	URoadConstructionWidget* RoadWidget;

	UPROPERTY(EditDefaultsOnly, Category="Widget")
	TSubclassOf<UBuildingConstructionWidget> BuildingWidgetClass;

	UPROPERTY(VisibleDefaultsOnly, Category="Widget")
	UBuildingConstructionWidget* BuildingWidget;

	UPROPERTY(EditDefaultsOnly, Category="Widget")
	TSubclassOf<UInteriorDesignWidget> InteriorWidgetClass;

	UPROPERTY(VisibleDefaultsOnly, Category="Widget")
	UInteriorDesignWidget* InteriorWidget;

	//Update Mode Helpers
	void UpdateUI();
	void UpdateArchVizMode();
	void SetArchVizMode(IArchVizMode* NewArchVizMode);
};
