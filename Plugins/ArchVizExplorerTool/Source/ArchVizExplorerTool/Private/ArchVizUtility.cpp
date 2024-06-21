// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizUtility.h"

FVector ArchVizUtility::GetSnappedLocation(const FVector& WorldLocation, const FVector& GridSpacing) {
	float SnapX = FMath::RoundToFloat(WorldLocation.X / GridSpacing.X) * GridSpacing.X;
	float SnapY = FMath::RoundToFloat(WorldLocation.Y / GridSpacing.Y) * GridSpacing.Y;
	float SnapZ = FMath::RoundToFloat(WorldLocation.Z / GridSpacing.Z) * GridSpacing.Z;

	return FVector(SnapX, SnapY, SnapZ);
}