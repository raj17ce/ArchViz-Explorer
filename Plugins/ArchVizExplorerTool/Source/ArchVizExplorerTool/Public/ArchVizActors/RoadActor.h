// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/SplineComponent.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMeshGenerator.h"
#include "Components/SplineMeshComponent.h"
#include "ArchVizActor.h"
#include "Materials/MaterialInterface.h"
#include "RoadActor.generated.h"

DECLARE_DELEGATE_OneParam(FOnRoadActorStateChanged, ERoadActorState)

UENUM(BlueprintType)
enum class ERoadActorState : uint8 {
	None,
	Selected,
	Generating
};

enum class ERoadType : uint8 {
	Sharp,
	Curved
};

UCLASS()
class ARCHVIZEXPLORERTOOL_API ARoadActor : public AArchVizActor {
	GENERATED_BODY()

public:
	friend class URoadConstructionMode;
	// Sets default values for this actor's properties
	ARoadActor();

	virtual void Tick(float DeltaTime) override;

	void AddSplinePoint(const FVector& Location);
	void RemoveLastSplinePoint();

	UFUNCTION(BlueprintCallable)
	void UpdateRoad();

	void SetState(ERoadActorState NewState);
	ERoadActorState GetState() const;

	void SetRoadType(ERoadType NewRoadType);
	ERoadType GetRoadType() const;

	virtual void ShowWidget() override;
	virtual void HideWidget() override;

	void HandleStateChange();

	FOnRoadActorStateChanged OnRoadActorStateChanged;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	USceneComponent* SceneComponent;

	UPROPERTY()
	USplineComponent* SplineComponent;

	UPROPERTY()
	TArray<USplineMeshComponent*> SplineMeshComponents;

	UPROPERTY()
	TArray<FVector> SplinePoints;

	UPROPERTY(EditDefaultsOnly, Category = "Road")
	UStaticMesh* RoadMesh;

	float Width;

	ERoadActorState State;
	ERoadType RoadType;

	UPROPERTY()
	UMaterialInterface* Material;
private:
	void DestroyRoadSegments();
	void HandleMaterialChange(FMaterialAssetData MaterialData);
};
