// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizMode.h"
#include "UObject/NoExportTypes.h"
#include "ArchVizActors/RoadActor.h"
#include "InputMappingContext.h"
#include "Widgets/RoadConstructionWidget.h"
#include "RoadConstructionMode.generated.h"

UENUM(BlueprintType)
enum class ERoadModeState : uint8 {
    Free,
    NewObject
};

/**
 * 
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORERTOOL_API URoadConstructionMode : public UArchVizMode {
	GENERATED_BODY()

public:
    virtual void Setup() override;
    virtual void Cleanup() override;
    virtual void EnterMode() override;
    virtual void ExitMode() override;
    virtual void SetupInputComponent() override;

protected:
    UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARoadActor> RoadActorClass;

private:
    ERoadModeState RoadModeState;

    UPROPERTY()
    ARoadActor* CurrentRoadActor;

    void HandleLeftMouseClick();

    void BindPropertyWidgetDelegates();

    //Main-Widget

	UFUNCTION()
	void HandleSaveRoadButtonClick();

	UFUNCTION()
	void HandleUndoRoadButtonClick();

    //Property-Panel
    UFUNCTION()
	void HandleRoadWidthChange(float InWidth);

    UFUNCTION()
	void HandleRoadTypeChange(FString Selectedtype, ESelectInfo::Type SelectionType);

    UFUNCTION()
	void HandleRoadNewButtonClick();

	UFUNCTION()
	void HandleRoadDeleteButtonClick();

	UFUNCTION()
	void HandleRoadCloseButtonClick();

    //Actor
    void HandleRoadActorStateChange(ERoadActorState RoadActorState);
    
    void HandleFreeState();
    void HandleNewObjectState();
};
