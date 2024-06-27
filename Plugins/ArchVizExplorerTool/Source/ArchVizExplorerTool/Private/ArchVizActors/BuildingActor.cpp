// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizActors/BuildingActor.h"
#include "../Plugins/Runtime/ProceduralMeshComponent/Source/ProceduralMeshComponent/Public/ProceduralMeshComponent.h"

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

void ABuildingActor::ShowWidget() {
	if (IsValid(PropertyPanelWidget)) {
		PropertyPanelWidget->AddToViewport();
	}
}

void ABuildingActor::HideWidget() {
	if (IsValid(PropertyPanelWidget)) {
		PropertyPanelWidget->RemoveFromParent();
	}
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

void ABuildingActor::HighlightSelectedActor() {
	TSet<UActorComponent*> ActorComponents = GetComponents();

	for (auto& ActorComponent : ActorComponents) {
		if (auto* Component = Cast<UPrimitiveComponent>(ActorComponent)) {
			Component->SetRenderCustomDepth(true);
			Component->CustomDepthStencilValue = 2;
		}
	}
}

void ABuildingActor::UnhighlightDeselectedActor() {
	TSet<UActorComponent*> ActorComponents = GetComponents();

	for (auto& ActorComponent : ActorComponents) {
		if (auto* Component = Cast<UPrimitiveComponent>(ActorComponent)) {
			Component->SetRenderCustomDepth(false);
		}
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

void ABuildingActor::RotateActor(double Degree) {
	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Yaw = static_cast<int32>(CurrentRotation.Yaw + Degree) % 360;

	SetActorRotation(CurrentRotation);
}

