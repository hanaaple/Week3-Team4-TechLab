#pragma once
#include "Core/Container/Map.h"
#include "Core/Container/Pair.h"
#include "Core/Container/String.h"
#include "Core/Math/Vector.h"


struct FObjVertex
{
	float X, Y, Z;    // Position
	float R = 1, G = 1, B = 1, A = 1; // Color
	float U, V;		  // Texture UV
	float NX, NY, NZ;  // Normal

	bool operator==(const FObjVertex& Other) const
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
	bool operator!=(const FObjVertex& Other) const
	{
		return !(*this == Other);
	}
};

struct FObjMaterialInfo
{
	FString MaterialName;	// newmtl Blatt_NONE
	
	float SpecularExponent = 50.0;			// Ns
	FVector Ambient = FVector(1);				// Ka
	FVector Diffuse = FVector(1);				// Kd
	FVector Specular = FVector(1);				// Ks
	FVector Emissive = FVector(0);				// Ke

	float OpticalDensity = 1;			// Ni
	float Dissolve = 1;					// d     Dissolve / Transparency
	int32 Illumination = 2;			// illum

	// File Path
	FString AmbientMap;				// Ka
	FString DiffuseMap;				// Nd
	FString SpecularMap;			// Ks
	FString SpecularExponentMap;	// Ns
	FString DissolveMap;			// d
	FString BumpMap;				// bump

	bool operator==(const FObjMaterialInfo& Other) const
	{
		return MaterialName == Other.MaterialName;
	}
	
	bool operator!=(const FObjMaterialInfo& Other) const
	{
		return !(*this == Other);
	}
};

// 일단 object는 여러개여도 하나로
// Raw Data
struct FObjInfo
{
	FString PathFileName;

	// VertexData
	TArray<FVector> VertexList;
	TArray<FVector> NormalList;
	TArray<TPair<float, float>> UVList;

	// Face
	TArray<uint32> VertexIndexList;
	TArray<uint32> NormalIndexList;
	TArray<uint32> UvIndexList;

	// 머티리얼 중복 X
	TArray<FObjMaterialInfo> MaterialDataMap;	// Material Map       중복 이름 X 



	// Section의 Material 이름과 Index	(usemtl MaterialName)
	TArray<TPair<FString, int32>> MaterialIndexData;
};

struct FStaticMesh
{
	FString PathFileName;
	FString Name;
	TArray<FObjVertex> Vertices;
	TMap<FString, TArray<uint32>> IndexDataList;
	TArray<FObjMaterialInfo> MaterialData;
};


class ObjReader
{
public:
	static FStaticMesh* Read(const FString& InFilePath);
	
protected:
	static bool CheckFile(const FString& InFilePath);

	static TArray<FObjMaterialInfo> ReadMtl(const FString& FilePath);
	static FObjInfo ReadRawData(const FString& InFilePath);
	static FStaticMesh* CookRawData(FObjInfo InObjInfo);
};