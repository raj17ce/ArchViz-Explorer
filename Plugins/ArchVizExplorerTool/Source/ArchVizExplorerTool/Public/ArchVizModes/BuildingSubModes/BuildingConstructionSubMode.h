// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class ARCHVIZEXPLORERTOOL_API IBuildingConstructionSubMode {
public:
	virtual ~IBuildingConstructionSubMode() = default;

	virtual void Setup() = 0;
	virtual void EnterSubMode() = 0;
	virtual void ExitSubMode() = 0;
	virtual void SetPlayerController(APlayerController* Controller) = 0;
	virtual void SetupInputComponent() = 0;
};
