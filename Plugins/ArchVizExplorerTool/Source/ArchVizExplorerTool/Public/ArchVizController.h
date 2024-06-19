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
	void HandleControllerModeChange(EArchVizMode NewArchVizMode);

protected:
	
	//Road
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARoadActor> RoadActorClass;

	//Wall
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWallActor> WallActorClass;

private:
	FInputModeGameAndUI InputModeGameAndUI{};

	EArchVizMode CurrentArchVizMode;

	//Road
	UPROPERTY(VisibleDefaultsOnly)
	UInputMappingContext* RoadMappingContext;

	UPROPERTY(VisibleDefaultsOnly)
	ARoadActor* RoadActor;

	void SetupRoadInputComponent();

	//Wall
	UPROPERTY(VisibleDefaultsOnly)
	UInputMappingContext* WallMappingContext;

	UPROPERTY(VisibleDefaultsOnly)
	AWallActor* WallActor;

	void SetupWallInputComponent();

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

	//Road Input Handlers
	void HandleRoadLeftMouseClick();

	//Wall Input Handlers
	void HandleWallLeftMouseClick();
	void HandleWallRKeyPress();

	//Update Mode Helpers
	void UpdateUI();
	void UpdateMappingContext();

	//Utility
	FVector SnapToGrid(FVector WorldLocation);
	FHitResult GetHitResult() const;
};
