// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteriorActor.generated.h"

UENUM(BlueprintType)
enum class EInteriorActorState : uint8 {
	None,
	Preview,
	Selected,
	Moving
};

UENUM(BlueprintType)
enum class EInteriorAssetType : uint8 {
	FloorPlaceable,
	WallPlaceable,
	RoofPlaceable
};

UCLASS()
class ARCHVIZEXPLORERTOOL_API AInteriorActor : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteriorActor();

	void SetState(EInteriorActorState NewInteriorActorState);
	EInteriorActorState GetState() const;

	void SetInteriorAssetType(EInteriorAssetType NewInteriorAssetType);
	EInteriorAssetType GetInteriorAssetType() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	EInteriorActorState InteriorActorState;
	EInteriorAssetType InteriorAssetType;
};
