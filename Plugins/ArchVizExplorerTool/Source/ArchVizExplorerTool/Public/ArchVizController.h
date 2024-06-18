// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "RoadActor.h"
#include "ControllerModeEnum.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "Widgets/ControllerModeWidget.h"
#include "Widgets/RoadConstructionWidget.h"
#include "Widgets/BuildingConstructionWidget.h"
#include "Widgets/InteriorDesignWidget.h"
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
	virtual void SetupInputComponent() override;

	UFUNCTION()
	void HandleControllerModeChange(EControllerMode NewControllerMode);

protected:
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARoadActor> RoadActorClass;

private:
	UPROPERTY(VisibleDefaultsOnly)
	UInputMappingContext* RoadMappingContext;

	UPROPERTY(VisibleDefaultsOnly)
	ARoadActor* RoadActor;

	FInputModeGameAndUI InputModeGameAndUI{};

	UPROPERTY(EditDefaultsOnly, Category="Widget")
	TSubclassOf<UControllerModeWidget> ControllerModeWidgetClass;

	UPROPERTY(VisibleDefaultsOnly, Category="Widget")
	UControllerModeWidget* ControllerModeWidget;

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

	void HandleRoadLeftMouseClick();

	void UpdateUI(EControllerMode NewControllerMode);
	void UpdateMappingContext(EControllerMode NewControllerMode);

	FVector SnapToGrid(FVector WorldLocation);
};
