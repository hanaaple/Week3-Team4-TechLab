#pragma once
#include <memory>
#include <string>
#include <queue>

#include "Core/Container/String.h"
#include "Core/HAL/PlatformType.h"
#include "Core/Math/Vector.h"


struct UObjectInfo
{
	FVector Location;
	FVector Rotation;
	FVector Scale;
	FString ObjectType;

	uint32 UUID;
};

struct ACameraInfo
{
	FVector Location;
	FVector Rotation;
	float FieldOfView;
	float NearClip;
	float FarClip;
};

struct UWorldInfo
{
	//UObjectInfo** ObjctInfos;
	std::queue<std::unique_ptr<UObjectInfo>> ObjectInfos;
	uint32 ActorCount;
	ACameraInfo CameraInfo;
	uint32 Version;
	std::string SceneName;
};

class JsonSaveHelper
{
public:
	// SceneName - 확장자 제외
	static std::unique_ptr<UWorldInfo> LoadScene(const std::string& SceneName);
	static void SaveScene(UWorldInfo WorldInfo);
};
