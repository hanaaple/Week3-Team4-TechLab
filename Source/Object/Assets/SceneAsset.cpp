#include "SceneAsset.h"
#include "SimpleJSON/Json.hpp"
#include "filesystem"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

bool USceneAsset::RegisterAsset()
{
	return true;
}

bool USceneAsset::Load()
{
	if (IsLoaded())
	{
		cout << "Asset already loaded: " << MetaData.GetAssetPath().GetData() << endl;
		return true;
	}

	filesystem::path filePath = MetaData.GetAssetPath().GetData();
	if (filesystem::exists(filePath) == false)
	{
		cout << "File not found: " << filePath << endl;
		return false;
	}

	ifstream file(filePath);
	if (file.is_open() == false)
	{
		cout << "File open failed: " << filePath << endl;
		return false;
	}

	ostringstream oss;
	oss << file.rdbuf();
	string jsonStr = oss.str();

	json::JSON sceneData = json::JSON::Load(jsonStr);

	uint64 version = sceneData["Version"].ToInt();
	uint64 nextUUID = sceneData["NextUUID"].ToInt();

	json::JSON primitives = sceneData["Primitives"];

	TMap<FString, Primitive> loadedPrimitives;
	auto primitivesObj = primitives.ObjectRange();
	for (auto it = primitivesObj.begin(); it != primitivesObj.end(); ++it)
	{
		FString key = it->first;
		json::JSON primitive = it->second;

		Primitive loadedPrimitive;
		loadedPrimitive.Type = primitive["Type"].ToString();

		auto locationArr = primitive["Location"].ArrayRange();
		for (const auto& location : locationArr)
		{
			float value = location.ToFloat();
			loadedPrimitive.Location.Add(value);
		}

		auto rotationArr = primitive["Rotation"].ArrayRange();
		for (const auto& rotation : rotationArr)
		{
			float value = rotation.ToFloat();
			loadedPrimitive.Rotation.Add(value);
		}

		auto scaleArr = primitive["Scale"].ArrayRange();
		for (const auto& scale : scaleArr)
		{
			float value = scale.ToFloat();
			loadedPrimitive.Scale.Add(value);
		}

		loadedPrimitives[key] = loadedPrimitive;
	}

	SceneData = FSceneData(version, nextUUID, loadedPrimitives);

	MetaData.SetIsLoaded(true);
	return true;
}

bool USceneAsset::Save(FString path)
{
	json::JSON sceneData = json::JSON::Make(json::JSON::Class::Object);

	sceneData["Version"] = static_cast<uint32>(SceneData.Version);
	sceneData["NextUUID"] = static_cast<uint32>(SceneData.NextUUID);

	json::JSON primitives = json::JSON::Make(json::JSON::Class::Object);
	for (const auto& primitive : SceneData.Primitives)
	{
		json::JSON primitiveData = json::JSON::Make(json::JSON::Class::Object);

		json::JSON location = json::JSON::Make(json::JSON::Class::Array);
		for (const auto& value : primitive.Value.Location)
		{
			location.append(value);
		}
		primitiveData["Location"] = location;

		json::JSON rotation = json::JSON::Make(json::JSON::Class::Array);
		for (const auto& value : primitive.Value.Rotation)
		{
			rotation.append(value);
		}
		primitiveData["Rotation"] = rotation;

		json::JSON scale = json::JSON::Make(json::JSON::Class::Array);
		for (const auto& value : primitive.Value.Scale)
		{
			scale.append(value);
		}
		primitiveData["Scale"] = scale;

		primitives.append(primitive.Key.GetData(), primitiveData);

		primitiveData["Type"] = primitive.Value.Type.GetData();
	}

	sceneData["Primitives"] = primitives;


	string jsonStr = sceneData.dump(2);

	if (path.IsEmpty())
	{
		path = MetaData.GetAssetPath();
	}

	ofstream file(path.GetData());
	if (!file.is_open())
	{
		cout << "File open failed: " << path.GetData() << endl;
		return false;
	}

	file << jsonStr;
	file.close();

	return true;
}

bool USceneAsset::Unload()
{
	return true;
}
