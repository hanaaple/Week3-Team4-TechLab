#pragma once
#include "Core/AbstractClass/Singleton.h"
#include "Core/Container/Map.h"
#include "Asset.h"
#include "Object/ObjectFactory.h"
#include <iostream>


class UAssetManager : public TSingleton<UAssetManager>
{
public:
	void RegisterAssetMetaDatas();
	void RegisterAssetMetaData(const std::string& name, const std::string& path, uint64 size, const std::string& extension);

	template<typename T>
		requires std::derived_from<T, UAsset>
	T* FindAsset(std::string& name)
	{
		T* asset = Assets.Find(name);
		if (asset == nullptr)
		{
			std::cout << "Can't found Asset :" << name << std::endl;
			return nullptr;
		}

		return dynamic_cast<T*>(asset);
	}

	void LoadAssets();

	template <typename T>
		requires std::derived_from<T, UAsset>
	T* LoadAsset(FAssetMetaData metaData)
	{
		T* asset = FindAsset<T>(metaData.GetAssetName());
		if (asset != nullptr)
		{
			std::cout << "Already Loaded " << metaData.GetAssetName() << std::endl;
			return asset;
		}

		asset = FObjectFactory::ConstructObject<T>();
		asset->SetMetaData(metaData);
		if (asset->Load())
		{
			asset->GetMetaData().SetLoaded(true);
			Assets[metaData.GetAssetName()] = asset;
			return asset;
		}
		else
		{
			delete asset;
			std::cout << "Load Failed: " << metaData.GetAssetName() << std::endl;
			return nullptr;
		}
	}

	template <typename T>
		requires std::derived_from<T, UAsset>
	void UnloadAsset(const std::string& name)
	{
		T* asset = FindAsset<T>(name);
		if (asset != nullptr)
		{
			asset->Unload();
			Assets.Remove(name);
			delete asset;
		}
	}

	template <typename T>
		requires std::derived_from<T, UAsset>
	void SaveAsset(const std::string& name, const std::string& path)
	{
		T* asset = FindAsset<T>(name);
		if (asset == nullptr)
		{
			std::cout << "Can't found Asset: " << name << std::endl;
			return;
		}

		if (asset != nullptr)
		{
			if (asset->Save(path) == true)
			{
				std::cout << "Save Success: " << name << std::endl;
			}
			else
			{
				std::cout << "Save Failed: " << name << std::endl;
			}
		}
	}
private:
	TMap<std::string, FAssetMetaData> AssetMetaDatas;
	TMap<std::string, UAsset*> Assets;
};

