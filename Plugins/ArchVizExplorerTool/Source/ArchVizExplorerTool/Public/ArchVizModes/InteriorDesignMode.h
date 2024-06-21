// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchVizMode.h"
#include "UObject/NoExportTypes.h"
#include "InputMappingContext.h"
#include "InteriorDesignMode.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class ARCHVIZEXPLORERTOOL_API UInteriorDesignMode : public UObject, public IArchVizMode {
	GENERATED_BODY()

public:
    virtual void Setup() override;
    virtual void EnterMode() override;
    virtual void ExitMode() override;
    virtual void SetPlayerController(APlayerController* Controller) override;
    virtual void SetupInputComponent() override;

private:
    UPROPERTY()
    APlayerController* PlayerController;

    UPROPERTY()
	UInputMappingContext* MappingContext;
};
