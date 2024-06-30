// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizMode.h"
#include "UObject/NoExportTypes.h"
#include "InputMappingContext.h"
#include "ArchVizActors/InteriorActor.h"
#include "DataAssets/InteriorDataAsset.h"
#include "InteriorDesignMode.generated.h"

UENUM(BlueprintType)
enum class EInteriorModeState : uint8 {
	Free,
    OldObject,
    NewObject
};


/**
 *
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORERTOOL_API UInteriorDesignMode : public UArchVizMode {
	GENERATED_BODY()

public:
    virtual void Setup() override;
    virtual void EnterMode() override;
    virtual void ExitMode() override;
    virtual void SetPlayerController(APlayerController* Controller) override;
    virtual void SetupInputComponent() override;

    void Cleanup();
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<AInteriorActor> InteriorActorClass;

private:
    EInteriorModeState InteriorModeState;

    UPROPERTY()
    AInteriorActor* CurrentInteriorActor;

    UFUNCTION()
    void HandleInteriorAssetSelect(FInteriorAssetData AssetData);

    void HandleLeftMouseClick();
    void HandleRKeyPress();
    void HandleMKeyPress();

    void BindPropertyWidgetDelegates();

    UFUNCTION()
	void HandleInteriorNewButtonClick();

	UFUNCTION()
	void HandleInteriorDeleteButtonClick();

	UFUNCTION()
	void HandleInteriorCloseButtonClick();

    void HandleFreeState();
    void HandleOldObjectState();
    void HandleNewObjectState();
};
