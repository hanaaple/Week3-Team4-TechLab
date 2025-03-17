#pragma once
#include "Asset.h"
#include "Core/Container/Array.h"
#include "Core/Container/Map.h"

struct Primitive 
{
	Primitive(TArray<float> InLocation, TArray<float> InRotation, TArray<float> InScale, FString InType)
		: Location(InLocation), Rotation(InRotation), Scale(InScale), Type(InType)
	{
	}

	Primitive()
	{
	}

	Primitive(const Primitive& Other)
	{
		Location = Other.Location;
		Rotation = Other.Rotation;
		Scale = Other.Scale;
		Type = Other.Type;
	}

	// 3D 좌표 정보: 위치, 회전, 스케일
	TArray<float> Location;
	TArray<float> Rotation;
	TArray<float> Scale;
	// 프리미티브 타입 (예: "Sphere", "Cube", "Triangle")
	FString Type;
};

struct FSceneData
{
	explicit FSceneData(uint64 InVersion, uint64 InNextUUID, TMap<FString, Primitive> InPrimitives)
		: Version(InVersion), NextUUID(InNextUUID), Primitives(InPrimitives)
	{
	}

	FSceneData()
		: Version(0), NextUUID(0)
	{
	}

	uint64 Version;
	uint64 NextUUID;
	TMap<FString, Primitive> Primitives;
};


class USceneAsset : public UAsset
{
	DECLARE_CLASS(USceneAsset, UAsset)

public:
	USceneAsset() : UAsset() {}
	virtual ~USceneAsset() = default;
	virtual bool RegisterAsset() override;
	virtual bool Load() override;
	virtual bool Save(FString path = "") override;
	virtual bool Unload() override;

private:
	FSceneData SceneData;
};

