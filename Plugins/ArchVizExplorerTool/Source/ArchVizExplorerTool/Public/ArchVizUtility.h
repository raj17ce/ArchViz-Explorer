// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class ARCHVIZEXPLORERTOOL_API ArchVizUtility {
public:
	static FVector GetSnappedLocation(const FVector& WorldLocation, const FVector& GridSpacing = FVector{100.0, 100.0, 1.0});
	static FVector GetFloorSnappedLocation(const FVector& WorldLocation, const FVector& GridSpacing = FVector{100.0});
	static FVector GetCeilSnappedLocation(const FVector& WorldLocation, const FVector& GridSpacing = FVector{100.0});
};
