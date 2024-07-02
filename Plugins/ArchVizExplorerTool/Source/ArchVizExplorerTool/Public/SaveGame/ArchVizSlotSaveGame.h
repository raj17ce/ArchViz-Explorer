// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ArchVizSlotSaveGame.generated.h"

/**
 *
 */
UCLASS()
class ARCHVIZEXPLORERTOOL_API UArchVizSlotSaveGame : public USaveGame {
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FString> SlotsNames;
};
