#pragma once
#include "Core//UObject/Object.h"
#include "Core/EngineTypes.h"
#include <string>

enum class EAssetType
{
	Texture,
	Material,
	Mesh,
	Animation,
	Sound,
	Font,
	Shader,
	Blueprint,
	Scene,
	World,
	Asset,
	EditorConfig,
	None
};

struct FAssetMetaData
{
public:
	FAssetMetaData() : AssetName(""), AssetPath(""), AssetSize(0), Type(EAssetType::None), BIsLoaded(false), BIsDirty(false) {}

	FAssetMetaData(std::string InAssetName, std::string InAssetPath, uint64 assetSize, std::string extension)
		: AssetName(InAssetName), AssetPath(InAssetPath), AssetSize(assetSize), BIsLoaded(false), BIsDirty(false)
	{
		if (extension == ".png" || extension == ".jpb" || extension == ".bmp" || extension == ".dds")
		{
			Type = EAssetType::Texture;
		}
		else if (extension == ".fbx" || extension == ".obj")
		{
			Type = EAssetType::Mesh;
		}
		else if (extension == ".wav" || extension == ".mp3")
		{
			Type = EAssetType::Sound;
		}
		else if (extension == ".ttf" || extension == ".otf")
		{
			Type = EAssetType::Font;
		}
		else if (extension == ".material")
		{
			Type = EAssetType::Material;
		}
		else if (extension == ".anim")
		{
			Type = EAssetType::Animation;
		}
		else if (extension == ".hlsl")
		{
			Type = EAssetType::Shader;
		}
		else if (extension == ".blueprint")
		{
			Type = EAssetType::Blueprint;
		}
		else if (extension == ".Scene")
		{
			Type = EAssetType::Scene;
		}
		else if (extension == ".world")
		{
			Type = EAssetType::World;
		}
		else if (extension == ".ini")
		{
			Type = EAssetType::EditorConfig;
		}
		else
		{
			Type = EAssetType::None;
		}
	}

public:
	std::string GetAssetName() const { return AssetName; }
	std::string GetAssetPath() const { return AssetPath; }
	uint64 GetAssetSize() const { return AssetSize; }
	EAssetType GetAssetType() const { return Type; }
	bool IsLoaded() const { return BIsLoaded; }
	bool IsDirty() const { return BIsDirty; }

	void SetAssetName(std::string InAssetName) { AssetName = InAssetName; }
	void SetAssetPath(std::string InAssetPath) { AssetPath = InAssetPath; }
	void SetAssetSize(uint64 InAssetSize) { AssetSize = InAssetSize; }
	void SetAssetType(EAssetType InAssetType) { Type = InAssetType; }
	void SetIsLoaded(bool bIsLoaded) { BIsLoaded = bIsLoaded; }
	void SetIsDirty(bool bIsDirty) { BIsDirty = bIsDirty; }

private:
	std::string AssetName;
	std::string AssetPath;
	uint64 AssetSize;
	EAssetType Type;

	bool BIsLoaded;
	bool BIsDirty;
};

class UAsset : public UObject
{
public:
	UAsset() : UObject() {}
	virtual ~UAsset();

	virtual bool RegisterAsset() = 0;
	virtual bool Load() = 0;
	virtual bool Save(std::string path = "") = 0;
	virtual bool Unload() = 0;

	inline void SetMetaData(FAssetMetaData InMetaData) { MetaData = InMetaData; }
	inline void SetAssetName(std::string InAssetName) { MetaData.SetAssetName(InAssetName); }
	inline void SetAssetPath(std::string InAssetPath) { MetaData.SetAssetPath(InAssetPath); }
	inline void SetIsLoaded(bool bIsLoaded) { MetaData.SetIsLoaded(bIsLoaded); }
	inline void SetIsDirty(bool bIsDirty) { MetaData.SetIsDirty(bIsDirty); }

	inline bool IsLoaded() const { return MetaData.IsLoaded(); }
	inline bool IsDirty() const { return MetaData.IsDirty(); }

	inline std::string GetAssetName() const { return MetaData.GetAssetName(); }
	inline std::string GetAssetPath() const { return MetaData.GetAssetPath(); }
	inline uint64 GetAssetSize() const { return MetaData.GetAssetSize(); }
	inline EAssetType GetAssetType() const { return MetaData.GetAssetType(); }

protected:
	FAssetMetaData MetaData;
};

