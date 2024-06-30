// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Widgets/PropertyPanelWidget.h"
#include "Widgets/MaterialWidget.h"
#include "ArchVizActor.generated.h"

UCLASS()
class ARCHVIZEXPLORERTOOL_API AArchVizActor : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AArchVizActor();

	virtual void ShowWidget();
	virtual void HideWidget();

	virtual void HighlightSelectedActor();
	virtual void UnhighlightDeselectedActor();

	virtual void RotateActor(double Degree);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual FHitResult GetHitResult(const TArray<AActor*>& ActorsToIgnore = TArray<AActor*>{}) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPropertyPanelWidget> PropertyPanelWidgetClass;
	
	UPROPERTY()
	UPropertyPanelWidget* PropertyPanelWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMaterialWidget> MaterialWidgetClass;
	
	UPROPERTY()
	UMaterialWidget* MaterialWidget;
};
