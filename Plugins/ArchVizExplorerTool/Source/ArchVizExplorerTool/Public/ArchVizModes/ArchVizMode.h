// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class APlayerController;

/**
 *
 */
class ARCHVIZEXPLORERTOOL_API IArchVizMode {
public:
	virtual ~IArchVizMode() = default;

	virtual void EnterMode() = 0;
	virtual void ExitMode() = 0;
	virtual void SetPlayerController(APlayerController* Controller) = 0;
	virtual void SetupInputComponent() = 0;
};
