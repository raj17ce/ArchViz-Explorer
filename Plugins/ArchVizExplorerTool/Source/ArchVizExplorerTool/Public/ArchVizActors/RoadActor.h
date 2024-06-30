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
#include "RoadActor.generated.h"

UCLASS()
class ARCHVIZEXPLORERTOOL_API ARoadActor : public AArchVizActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARoadActor();

	virtual void Tick(float DeltaTime) override;

	void AddSplinePoint(const FVector& Location);

	UFUNCTION(BlueprintCallable)
	void GenerateRoad();

	virtual void ShowWidget() override;
	virtual void HideWidget() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	USceneComponent* SceneComponent;

	UPROPERTY()
	USplineComponent* SplineComponent;

	UPROPERTY()
	TArray<USplineMeshComponent*> SplineMeshComponents;

	UPROPERTY(EditDefaultsOnly, Category = "Road")
	UStaticMesh* RoadMesh;

private:
	void GenerateRoadSegment(const FVector& StartLocation, const FVector& StartTangent, const FVector& EndLocation, const FVector& EndTangent);
	void DestroyRoadSegments();
};
