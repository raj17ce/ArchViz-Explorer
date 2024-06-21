// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizMode.h"
#include "UObject/NoExportTypes.h"
#include "ArchVizActors/RoadActor.h"
#include "InputMappingContext.h"
#include "Widgets/RoadConstructionWidget.h"
#include "RoadConstructionMode.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORERTOOL_API URoadConstructionMode : public UArchVizMode {
	GENERATED_BODY()

public:
    virtual void Setup() override;
    virtual void EnterMode() override;
    virtual void ExitMode() override;
    virtual void SetupInputComponent() override;

protected:
    UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARoadActor> RoadActorClass;

private:
	UPROPERTY()
    ARoadActor* RoadActor;

    void HandleLeftMouseClick();
};
