#pragma once
#include "Core/Container/String.h"
#include "Core/Math/Vector.h"


struct FNormalVertex
{
	float X, Y, Z;    // Position
	float R = 1, G = 1, B = 1, A = 1; // Color
	float U, V;		  // Texture UV
	float NX, NY, NZ;  // Normal

	bool operator==(const FNormalVertex& Other) const
	{
		float epsilon = std::numeric_limits<float>::epsilon();
		if ((FVector(X, Y, Z) - FVector{Other.X, Other.Y, Other.Z}).Length() < epsilon && (FVector(NX, NY, NZ) - FVector{Other.NX, Other.NY, Other.NZ}).Length() < epsilon
			&& (FVector4(R, G, B, A) - FVector4{Other.R, Other.G, Other.B, Other.A}).Length() < epsilon
			&& FMath::Abs(U - Other.U) < epsilon && FMath::Abs(V - Other.V) < epsilon)
		{
			return true;
		}
		return false;
	}
	bool operator!=(const FNormalVertex& Other) const
	{
		return !(*this == Other);
	}
};

struct FStaticMesh
{
	FString PathFileName;

	TArray<FNormalVertex> Vertices;
	TArray<uint32> Indices;
};

// Raw Data
struct FObjInfo
{
	// Vertex Index List
	// UV Index List
	// Normal Index List
	// Material List
	// Texture List

	TArray<FVector> Vertices;
	TArray<FVector> Normals;
	TArray<TArray<float>> UVs;
	TArray<TArray<TArray<uint32>>> Faces;
};

struct FObjMaterialInfo
{
	// Diffuse Scalar
	// Diffuse Texture
};


class ObjReader
{
public:
	static FStaticMesh Read(const FString& InFilePath);
	
protected:
	static bool CheckFile(const FString& InFilePath);

	static FObjInfo ReadRawData(const FString& InFilePath);
	static FStaticMesh CookRawData(FObjInfo ObjInfo);
};