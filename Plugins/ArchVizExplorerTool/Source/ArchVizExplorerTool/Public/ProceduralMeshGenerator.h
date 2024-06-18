// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"

/**
 * 
 */
class ARCHVIZEXPLORERTOOL_API ProceduralMeshGenerator {
public:
	ProceduralMeshGenerator();

	static void GenerateCube(UProceduralMeshComponent* ProceduralMeshComponent, int32 Section, const FVector& CubeDimensions, float ZOffset);
	
private:
	static void AddTriangle(TArray<int32>& Triangles, int32 Index1, int32 Index2, int32 Index3);
};
