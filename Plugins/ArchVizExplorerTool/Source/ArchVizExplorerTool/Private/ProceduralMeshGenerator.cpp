// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralMeshGenerator.h"
#include "Kismet/KismetMathLibrary.h"

ProceduralMeshGenerator::ProceduralMeshGenerator() {

}

void ProceduralMeshGenerator::GenerateCube(UProceduralMeshComponent* ProceduralMeshComponent, int32 Section, const FVector& CubeDimensions, float ZOffset) {
	//Vertices
	TArray<FVector> Vertices;

	//Front
	Vertices.Add(FVector{ -CubeDimensions.X / 2, -CubeDimensions.Y / 2, ZOffset + (CubeDimensions.Z / 2) }); //0
	Vertices.Add(FVector{ -CubeDimensions.X / 2, -CubeDimensions.Y / 2, ZOffset - (CubeDimensions.Z / 2) }); //1
	Vertices.Add(FVector{ -CubeDimensions.X / 2, CubeDimensions.Y / 2, ZOffset - (CubeDimensions.Z / 2) }); //2
	Vertices.Add(FVector{ -CubeDimensions.X / 2, CubeDimensions.Y / 2, ZOffset + (CubeDimensions.Z / 2) }); //3

	//Right
	Vertices.Add(FVector{ -CubeDimensions.X / 2, CubeDimensions.Y / 2, ZOffset + (CubeDimensions.Z / 2) }); //4
	Vertices.Add(FVector{ -CubeDimensions.X / 2, CubeDimensions.Y / 2, ZOffset - (CubeDimensions.Z / 2) }); //5
	Vertices.Add(FVector{ CubeDimensions.X / 2, CubeDimensions.Y / 2, ZOffset - (CubeDimensions.Z / 2) }); //6
	Vertices.Add(FVector{ CubeDimensions.X / 2, CubeDimensions.Y / 2, ZOffset + (CubeDimensions.Z / 2) }); //7

	//Back
	Vertices.Add(FVector{ CubeDimensions.X / 2, CubeDimensions.Y / 2, ZOffset + (CubeDimensions.Z / 2) }); //8
	Vertices.Add(FVector{ CubeDimensions.X / 2, CubeDimensions.Y / 2, ZOffset - (CubeDimensions.Z / 2) }); //9
	Vertices.Add(FVector{ CubeDimensions.X / 2, -CubeDimensions.Y / 2, ZOffset - (CubeDimensions.Z / 2) }); //10
	Vertices.Add(FVector{ CubeDimensions.X / 2, -CubeDimensions.Y / 2, ZOffset + (CubeDimensions.Z / 2) }); //11

	//Left
	Vertices.Add(FVector{ CubeDimensions.X / 2, -CubeDimensions.Y / 2, ZOffset + (CubeDimensions.Z / 2) }); //12
	Vertices.Add(FVector{ CubeDimensions.X / 2, -CubeDimensions.Y / 2, ZOffset - (CubeDimensions.Z / 2) }); //13
	Vertices.Add(FVector{ -CubeDimensions.X / 2, -CubeDimensions.Y / 2, ZOffset - (CubeDimensions.Z / 2) }); //14
	Vertices.Add(FVector{ -CubeDimensions.X / 2, -CubeDimensions.Y / 2, ZOffset + (CubeDimensions.Z / 2) }); //15

	//Top
	Vertices.Add(FVector{ CubeDimensions.X / 2, -CubeDimensions.Y / 2, ZOffset + (CubeDimensions.Z / 2) }); //16
	Vertices.Add(FVector{ -CubeDimensions.X / 2, -CubeDimensions.Y / 2, ZOffset + (CubeDimensions.Z / 2) }); //17
	Vertices.Add(FVector{ -CubeDimensions.X / 2, CubeDimensions.Y / 2, ZOffset + (CubeDimensions.Z / 2) }); //18
	Vertices.Add(FVector{ CubeDimensions.X / 2, CubeDimensions.Y / 2, ZOffset + (CubeDimensions.Z / 2) }); //19

	//Bottom
	Vertices.Add(FVector{ -CubeDimensions.X / 2, -CubeDimensions.Y / 2, ZOffset - (CubeDimensions.Z / 2) }); //20
	Vertices.Add(FVector{ CubeDimensions.X / 2, -CubeDimensions.Y / 2, ZOffset - (CubeDimensions.Z / 2) }); //21
	Vertices.Add(FVector{ CubeDimensions.X / 2, CubeDimensions.Y / 2, ZOffset - (CubeDimensions.Z / 2) }); //22
	Vertices.Add(FVector{ -CubeDimensions.X / 2, CubeDimensions.Y / 2, ZOffset - (CubeDimensions.Z / 2) }); //23

	//Triangles
	TArray<int32> Triangles;

	for (int32 i = 0; i < 24; i += 4) {
		AddTriangle(Triangles, i, i + 1, i + 2);
		AddTriangle(Triangles, i + 2, i + 3, i);
	}

	//Normals
	TArray<FVector> Normals;

	for (int32 i = 0; i < 24; i += 4) {
		FVector CurrentNormal = FVector::CrossProduct(UKismetMathLibrary::GetDirectionUnitVector(Vertices[i + 2], Vertices[i + 1]), UKismetMathLibrary::GetDirectionUnitVector(Vertices[i + 1], Vertices[i]));

		Normals.Add(CurrentNormal);
		Normals.Add(CurrentNormal);
		Normals.Add(CurrentNormal);
		Normals.Add(CurrentNormal);
	}

	//UVs
	TArray<FVector2D> UVs;

	for (int32 i = 0; i < 24; i += 4) {
		UVs.Add(FVector2D{ 0.0,0.0 });
		UVs.Add(FVector2D{ 0.0,1.0 });
		UVs.Add(FVector2D{ 1.0,1.0 });
		UVs.Add(FVector2D{ 1.0,0.0 });
	}

	if (IsValid(ProceduralMeshComponent)) {
		ProceduralMeshComponent->CreateMeshSection_LinearColor(Section, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>{}, TArray<FProcMeshTangent>{}, true);
	}
}

void ProceduralMeshGenerator::AddTriangle(TArray<int32>& Triangles, int32 Index1, int32 Index2, int32 Index3) {
	Triangles.Add(Index1);
	Triangles.Add(Index2);
	Triangles.Add(Index3);
}