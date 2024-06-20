// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizUtility.h"

FVector ArchVizUtility::GetSnappedLocation(FVector WorldLocation) {
	float GridSpacing = 100.0f;
	float SnapX = FMath::RoundToFloat(WorldLocation.X / GridSpacing) * GridSpacing;
	float SnapY = FMath::RoundToFloat(WorldLocation.Y / GridSpacing) * GridSpacing;
	float SnapZ = FMath::RoundToFloat(WorldLocation.Z / GridSpacing) * GridSpacing;

	return FVector(SnapX, SnapY, SnapZ);
}