// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizActors/BuildingActor.h"

// Sets default values
ABuildingActor::ABuildingActor() : State{ EBuildingActorState::None } {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ABuildingActor::SetState(EBuildingActorState NewState) {
	State = NewState;

	HandleStateChange();
}

EBuildingActorState ABuildingActor::GetState() const {
	return State;
}

void ABuildingActor::HandleStateChange() {
	if (State == EBuildingActorState::Selected) {
		ShowWidget();
		HighlightSelectedActor();
	}
	else {
		HideWidget();
		UnhighlightDeselectedActor();
	}
}

// Called when the game starts or when spawned
void ABuildingActor::BeginPlay() {
	Super::BeginPlay();

}

// Called every frame
void ABuildingActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}
