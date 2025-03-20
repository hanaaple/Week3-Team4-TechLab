#pragma once
#include "Asset.h"
#include "Core/Container/Array.h"
#include "Core/Container/Map.h"


struct Primitive 
{
	Primitive()	= default;
	Primitive(
		const TArray<float>& InLocation, const TArray<float>& InRotation, const TArray<float>& InScale,
		FString InType
	)
		: Location(InLocation)
		, Rotation(InRotation)
		, Scale(InScale)
		, Type(std::move(InType))
	{
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
	explicit FSceneData(uint64 InVersion, uint64 InNextUUID, const TMap<FString, Primitive>& InPrimitives)
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
	USceneAsset() = default;	
	virtual bool RegisterAsset() override;
	virtual bool Load() override;
	virtual bool Save(FString path = "") override;
	virtual bool Unload() override;

private:
	FSceneData SceneData;
};

