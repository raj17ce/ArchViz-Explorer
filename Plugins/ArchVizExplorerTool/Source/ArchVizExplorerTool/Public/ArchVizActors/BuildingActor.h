// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArchVizActor.h"
#include "Widgets/PropertyPanelWidget.h"
#include "BuildingActor.generated.h"

UENUM(BlueprintType)
enum class EBuildingActorState : uint8 {
	None,
	Selected,
	Preview,
	Generating,
	Moving
};

UCLASS()
class ARCHVIZEXPLORERTOOL_API ABuildingActor : public AArchVizActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuildingActor();

	void SetState(EBuildingActorState NewState);
	EBuildingActorState GetState() const;

	virtual void ShowWidget();
	virtual void HideWidget();

	void HandleStateChange();
	virtual void HighlightSelectedActor();
	virtual void UnhighlightDeselectedActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void RotateActor(double Degree);

	EBuildingActorState State;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPropertyPanelWidget> PropertyPanelWidgetClass;
	
	UPROPERTY()
	UPropertyPanelWidget* PropertyPanelWidget;
private:
	
};
