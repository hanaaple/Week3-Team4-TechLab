#include "AssetManager.h"
#include <filesystem>
#include <iostream>
#include "SceneAsset.h"

using namespace std;

void UAssetManager::RegisterAssetMetaDatas()
{
	if (IsDebuggerPresent())
	{
		filesystem::path curPath = filesystem::current_path();
		filesystem::path assetDir = curPath / "Contents";
		try
		{
			for (const auto& entry : filesystem::recursive_directory_iterator(assetDir))
			{
				if (filesystem::is_regular_file(entry.status()))
				{
					filesystem::path filePath = entry.path();

					// 파일의 확장자, 전체 경로, 파일 크기를 가져옵니다.
					string fileName = filePath.stem().string();
					string extension = filePath.extension().string();
					string fullPath = filePath.string();
					uint64 fileSize = filesystem::file_size(filePath);

					RegisterAssetMetaData(fileName, fullPath, fileSize, extension);
				}
			}
		}
		catch (std::filesystem::filesystem_error& e)
		{
			std::wcout << e.what() << std::endl;
		}
	}
	else
	{
		// 빌드된 실행 파일의 경로로 설정
	}
}

void UAssetManager::RegisterAssetMetaData(const std::string& name, const std::string& path, uint64 size, const std::string& extension)
{
	AssetMetaDatas[name] = FAssetMetaData(name, path, size, extension);
}

void UAssetManager::LoadAssets()
{
	for (auto& asset : AssetMetaDatas)
	{
		switch (asset.Value.GetAssetType())
		{
		case EAssetType::Scene:
		{
			USceneAsset* sceneAsset = FObjectFactory::ConstructObject<USceneAsset>();
			if (sceneAsset != nullptr)
			{
				sceneAsset->SetMetaData(asset.Value);
				sceneAsset->Load();
				Assets.Add(sceneAsset->GetAssetName(), sceneAsset);
			}
			else
			{
				cout << "Scene Asset Load Failed: " << asset.Value.GetAssetName() << endl;
			}
			break;
		}
		case EAssetType::EditorConfig:
		{
			break;
		}
		
		}
	}
}
