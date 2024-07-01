// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArchVizActor.h"
#include "DataAssets/InteriorDataAsset.h"
#include "InteriorActor.generated.h"

UENUM(BlueprintType)
enum class EInteriorActorState : uint8 {
	None,
	Preview,
	Selected,
	Moving
};

UCLASS()
class ARCHVIZEXPLORERTOOL_API AInteriorActor : public AArchVizActor {
	GENERATED_BODY()

public:
	friend class UInteriorDesignMode;
	// Sets default values for this actor's properties
	AInteriorActor();

	void SetState(EInteriorActorState NewState);
	EInteriorActorState GetState() const;

	void HandleStateChange();
	void SetActorAssetData(const FInteriorAssetData& NewAssetData);

	void AdjustWallInteriorRotation(FRotator& Rotation);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	EInteriorActorState State;
	
	FInteriorAssetData AssetData;

	UPROPERTY()
	USceneComponent* SceneComponent;

	UPROPERTY()
	UStaticMeshComponent* InteriorComponent;

	void HandlePreviewState();
	void HandleMovingState();
};
