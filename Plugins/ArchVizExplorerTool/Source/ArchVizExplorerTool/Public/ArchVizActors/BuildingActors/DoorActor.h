// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizActors/BuildingActor.h"
#include "DoorActor.generated.h"

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORERTOOL_API ADoorActor : public ABuildingActor {
	GENERATED_BODY()

public:
	friend class UDoorSubMode;

	// Sets default values for this actor's properties
	ADoorActor();

	bool GetIsOpen() const;
	void SetIsOpen(bool bIsDoorOpen);

	int32 GetParentWallComponentIndex() const;
	void SetParentWallComponentIndex(int32 NewIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Door")
	UStaticMesh* DoorFrameStaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Door")
	UStaticMesh* DoorStaticMesh;

private:
	UPROPERTY()
	USceneComponent* SceneComponent;

	UPROPERTY()
	UStaticMeshComponent* DoorFrameComponent;

	UPROPERTY()
	UStaticMeshComponent* DoorComponent;

	bool bIsOpen;
	int32 ParentWallComponentIndex;

	void HandlePreviewState();
	void HandleMovingState();

	virtual void ShowWidget() override;
	virtual void HideWidget() override;

	void OpenDoor();
	void CloseDoor();
};
