// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArchVizActor.h"
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ApplyPreviewMaterial();
	virtual void ApplyMaterial();

	void HandleStateChange();

	EBuildingActorState State;
};
