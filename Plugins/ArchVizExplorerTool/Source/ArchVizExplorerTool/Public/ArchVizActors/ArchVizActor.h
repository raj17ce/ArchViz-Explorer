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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UMaterialInterface* GetMaterial() const;
	void SetMaterial(UMaterialInterface* NewMaterial);

	int32 GetID() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ShowWidget();
	virtual void HideWidget();

	virtual void HighlightSelectedActor();
	virtual void UnhighlightDeselectedActor();

	virtual void RotateActor(double Degree);

	virtual FHitResult GetHitResult(const TArray<AActor*>& ActorsToIgnore = TArray<AActor*>{}) const;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPropertyPanelWidget> PropertyPanelWidgetClass;
	
	UPROPERTY()
	UPropertyPanelWidget* PropertyPanelWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMaterialWidget> MaterialWidgetClass;
	
	UPROPERTY()
	UMaterialWidget* MaterialWidget;

	UPROPERTY()
	UMaterialInterface* Material;

	int32 ID;

private:
	static inline int32 GeneratedID = 0;

	static int32 GenerateID() {
		return ++GeneratedID;
	}
};
