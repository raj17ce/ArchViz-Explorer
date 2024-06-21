// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArchVizActors/BuildingActor.h"
#include "WallActor.generated.h"

UCLASS()
class ARCHVIZEXPLORERTOOL_API AWallActor : public ABuildingActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWallActor();

	int32 GetSegmentIndex() const;
	void SetSegmentIndex(int32 Index);

	FRotator GetSegmentRotation() const;
	void SetSegmentRotation(const FRotator& Rotation);

	UPROPERTY(VisibleDefaultsOnly, Category = "Wall")
	UStaticMeshComponent* PreviewWallSegment;

	UPROPERTY(VisibleDefaultsOnly, Category = "Wall")
	TArray<UStaticMeshComponent*> WallSegments;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Wall")
	UStaticMesh* WallStaticMesh;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* SceneComponent;

private:
	int32 WallSegmentIndex;
	FRotator WallSegmentRotation;
};