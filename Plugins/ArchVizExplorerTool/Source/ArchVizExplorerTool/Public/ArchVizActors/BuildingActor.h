// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArchVizActor.h"
#include "BuildingActor.generated.h"

UCLASS()
class ARCHVIZEXPLORERTOOL_API ABuildingActor : public AArchVizActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuildingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void RotateActor(double Degree);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
