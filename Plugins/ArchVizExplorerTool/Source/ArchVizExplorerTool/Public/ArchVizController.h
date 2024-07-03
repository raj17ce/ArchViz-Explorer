// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
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
#include "ArchVizModes/SaveGameMode.h"
#include "Widgets/NotificationWidget.h"
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

	void AddErrorMessage(FText Message);
	void AddSuccessMessage(FText Message);

	void BindPropertyDelegatesToActor(AArchVizActor* Actor);

private:
	FInputModeGameAndUI InputModeGameAndUI{};

	EArchVizMode CurrentArchVizMode;

	UPROPERTY()
	UArchVizMode* CurrentArchVizModePtr;

	//Modes
	UPROPERTY(EditDefaultsOnly, Category="Mode")
	TSubclassOf<URoadConstructionMode> RoadConstructionModeClass;

	UPROPERTY()
	URoadConstructionMode* RoadConstructionMode;

	UPROPERTY(EditDefaultsOnly, Category="Mode")
	TSubclassOf<UBuildingConstructionMode> BuildingConstructionModeClass;

	UPROPERTY()
	UBuildingConstructionMode* BuildingConstructionMode;

	UPROPERTY(EditDefaultsOnly, Category="Mode")
	TSubclassOf<UInteriorDesignMode> InteriorDesignModeClass;

	UPROPERTY()
	UInteriorDesignMode* InteriorDesignMode;

	UPROPERTY(EditDefaultsOnly, Category="Mode")
	TSubclassOf<USaveGameMode> SaveGameModeClass;

	UPROPERTY()
	USaveGameMode* SaveGameMode;

	//Widgets
	UPROPERTY(EditDefaultsOnly, Category="Widget")
	TSubclassOf<UArchVizModeWidget> ArchVizModeWidgetClass;

	UPROPERTY()
	UArchVizModeWidget* ArchVizModeWidget;

	UPROPERTY(EditDefaultsOnly, Category="Widget")
	TSubclassOf<UNotificationWidget> NotificationWidgetClass;

	UPROPERTY()
	UNotificationWidget* NotificationWidget;

	//Update Mode Helpers
	void UpdateArchVizMode();
	void SetArchVizMode(UArchVizMode* NewArchVizModePtr);
};
