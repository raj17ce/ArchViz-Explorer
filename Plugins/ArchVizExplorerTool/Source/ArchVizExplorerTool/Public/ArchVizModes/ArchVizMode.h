// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "ArchVizMode.generated.h"

class UInputMappingContext;
class APlayerController;


/**
 *
 */
UCLASS(ABSTRACT)
class ARCHVIZEXPLORERTOOL_API UArchVizMode : public UObject {
	GENERATED_BODY()
public:
	//Pure Virtual Functions
	virtual void Setup() PURE_VIRTUAL(UArchVizMode::Setup, );
	virtual void Cleanup() PURE_VIRTUAL(UArchVizMode::Cleanup, );
	virtual void EnterMode() PURE_VIRTUAL(UArchVizMode::EnterMode, );
	virtual void ExitMode() PURE_VIRTUAL(UArchVizMode::ExitMode, );
	virtual void SetupInputComponent() PURE_VIRTUAL(UArchVizMode::SetupInputComponent, );

	//Virtual Functions
	virtual void SetPlayerController(APlayerController* Controller);
	virtual void ShowWidget();
	virtual void HideWidget();

	FHitResult GetHitResult(const TArray<AActor*>& ActorsToIgnore = TArray<AActor*>{}) const;
protected:
	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	UInputMappingContext* MappingContext;

	//Widget
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY()
	UUserWidget* Widget;
};
