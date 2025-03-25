#include "Core/Utils/ObjReader.h"

#include <fstream>
#include <sstream>

#include "Core/Container/String.h"
#include "Object/ObjectFactory.h"

FStaticMesh* ObjReader::Read(const FString& InFilePath)
{
	if (CheckFile(InFilePath))
	{
		FObjInfo ObjInfo = ReadRawData(InFilePath);
		FStaticMesh* StaticMeshAsset = CookRawData(ObjInfo);
		StaticMeshAsset->PathFileName = *InFilePath;
		return StaticMeshAsset;
	}
	return {};
}

bool ObjReader::CheckFile(const FString& InFilePath)
{
	std::ifstream f(*InFilePath);
	return f.good();
}

TArray<FObjMaterialInfo> ObjReader::ReadMtl(const FString& InFilePath)
{
	std::ifstream File;
	if (!File.is_open())
	{
		File.open(*InFilePath);
	}
	
	TArray<FObjMaterialInfo> MaterialInfoList;
	int32 Index = 0;

	std::string MaterialName;
	
	std::string Line; // std::string을 참조해야하므로, FString 대신 std::string 사용
	while (std::getline(File, Line))
	{		
		std::stringstream StringStream(Line);
		std::string Prefix;
		
		StringStream >> Prefix;

		if (Prefix == "newmtl")
		{
			if (!MaterialName.empty())
			{
				Index++;
			}
			
			MaterialInfoList.Add(FObjMaterialInfo());

			MaterialName = std::string((std::istreambuf_iterator<char>(StringStream)), std::istreambuf_iterator<char>());
			MaterialName.erase(0, MaterialName.find_first_not_of(" \t\n\r"));
			
			MaterialInfoList[Index].MaterialName = MaterialName;
		}
		else if (Prefix == "Ns")
		{
			float Value;
			StringStream >> Value;
			MaterialInfoList[Index].SpecularExponent = Value;
		}
		else if (Prefix == "Ni")
		{
			float Value;
			StringStream >> Value;
			MaterialInfoList[Index].OpticalDensity = Value;
		}
		else if (Prefix == "d")
		{
			float Value;
			StringStream >> Value;
			MaterialInfoList[Index].Dissolve = Value;
		}
		else if (Prefix == "illum")
		{
			uint32 Value;
			StringStream >> Value;
			MaterialInfoList[Index].Illumination = Value;
		}
		else if (Prefix == "Ka")
		{
			FVector Value;
			StringStream >> Value.X >> Value.Y >> Value.Z;
			MaterialInfoList[Index].Ambient = Value;
		}
		else if (Prefix == "Kd")
		{
			FVector Value;
			StringStream >> Value.X >> Value.Y >> Value.Z;
			MaterialInfoList[Index].Diffuse = Value;
		}
		else if (Prefix == "Ks")
		{
			FVector Value;
			StringStream >> Value.X >> Value.Y >> Value.Z;
			MaterialInfoList[Index].Specular = Value;
		}
		else if (Prefix == "Ke")
		{
			FVector Value;
			StringStream >> Value.X >> Value.Y >> Value.Z;
			MaterialInfoList[Index].Emissive = Value;
		}
		else if (Prefix == "map_Kd")
		{
			std::string Value = std::string((std::istreambuf_iterator<char>(StringStream)), std::istreambuf_iterator<char>());
			Value.erase(0, Value.find_first_not_of(" \t\n\r"));
			MaterialInfoList[Index].DiffuseMap = Value;
		}
		else if (Prefix == "map_Bump")
		{
			std::string Value = std::string((std::istreambuf_iterator<char>(StringStream)), std::istreambuf_iterator<char>());
			Value.erase(0, Value.find_first_not_of(" \t\n\r"));
			MaterialInfoList[Index].BumpMap = Value;
		}
		else if (Prefix == "map_Ks")
		{
			std::string Value = std::string((std::istreambuf_iterator<char>(StringStream)), std::istreambuf_iterator<char>());
			Value.erase(0, Value.find_first_not_of(" \t\n\r"));
			MaterialInfoList[Index].SpecularMap = Value;
		}
		else if (Prefix == "map_d")
		{
			std::string Value = std::string((std::istreambuf_iterator<char>(StringStream)), std::istreambuf_iterator<char>());
			Value.erase(0, Value.find_first_not_of(" \t\n\r"));
			MaterialInfoList[Index].DissolveMap = Value;
		}
		else if (Prefix == "map_Ns")
		{
			std::string Value = std::string((std::istreambuf_iterator<char>(StringStream)), std::istreambuf_iterator<char>());
			Value.erase(0, Value.find_first_not_of(" \t\n\r"));
			MaterialInfoList[Index].SpecularExponentMap = Value;
		}
		else if (Prefix == "map_Ka")
		{
			std::string Value = std::string((std::istreambuf_iterator<char>(StringStream)), std::istreambuf_iterator<char>());
			Value.erase(0, Value.find_first_not_of(" \t\n\r"));
			MaterialInfoList[Index].AmbientMap = Value;
		}
	}
	
	File.close();

	return MaterialInfoList;
}

FObjInfo ObjReader::ReadRawData(const FString& InFilePath)
{
	std::ifstream File;
	if (!File.is_open())
	{
		File.open(*InFilePath);
	}
	FObjInfo ObjInfo;
	ObjInfo.PathFileName = *InFilePath;

	std::string Line; // std::string을 참조해야하므로, FString 대신 std::string 사용
	while (std::getline(File, Line))
	{
		std::stringstream TokenStream(Line);
		std::string Prefix;

		TokenStream >> Prefix;

		if (Prefix == "o")
		{
			
			// ObjInfo.Add(ObjectName, FObjInfo());
		}
		else if (Prefix == "mtllib")
		{
			//mtllib Low-Poly Plant_.mtl
			std::string MtlLibPath((std::istreambuf_iterator<char>(TokenStream)), std::istreambuf_iterator<char>());
			MtlLibPath.erase(0, MtlLibPath.find_first_not_of(" \t\n\r"));
			
			TArray<FObjMaterialInfo> MaterialInfoList = ReadMtl(MtlLibPath);
			for (const auto& MaterialInfo : MaterialInfoList)
			{
				if (ObjInfo.MaterialDataMap.Find(MaterialInfo) == -1)
				{
					ObjInfo.MaterialDataMap.Add(MaterialInfo);
				}
			}
		}
		else if (Prefix == "usemtl")
		{
			// usemtl Blatt
			// 여기서부터 Material 변경
			std::string MaterialName;
			TokenStream >> MaterialName;
			ObjInfo.MaterialIndexData.Add({ MaterialName, ObjInfo.VertexIndexList.Num() });
		}
		else if (Prefix == "v")
		{
			FVector Pos;
			TokenStream >> Pos.X >> Pos.Y >> Pos.Z;
			ObjInfo.VertexList.Add(Pos);
		}
		else if (Prefix == "vt")
		{
			float u, v;
			TokenStream >> u >> v;
			
			ObjInfo.UVList.Add({u, v});
		}
		else if (Prefix == "vn")
		{
			FVector Normal;
			TokenStream >> Normal.X >> Normal.Y >> Normal.Z;
			ObjInfo.NormalList.Add(Normal);
		}
		else if (Prefix == "f")
		{
			for (int i = 0; i < 3; ++i)
			{
				std::string VertexData;
				TokenStream >> VertexData;
				std::replace(VertexData.begin(), VertexData.end(), '/', ' ');
				std::stringstream vss(VertexData);

				int posIdx, uvIdx, normIdx;
				vss >> posIdx >> uvIdx >> normIdx;
				ObjInfo.VertexIndexList.Add(posIdx - 1);
				ObjInfo.UvIndexList.Add(uvIdx - 1);
				ObjInfo.NormalIndexList.Add(normIdx - 1);
			}
		}
	}
	File.close();

	return ObjInfo;
}

FStaticMesh* ObjReader::CookRawData(FObjInfo InObjInfo)
{
	// 언리얼에서는 UActorFactoryStaticMesh라는 팩토리를 따로 만들어서 사용 (FStaticMesh가 UObject 상속은 안함)
	FStaticMesh* OutStaticMesh = new FStaticMesh();
	OutStaticMesh->PathFileName = InObjInfo.PathFileName;
	for (uint32 i = 0; i < InObjInfo.MaterialIndexData.Num(); i++)
	{
		FString MaterialName = InObjInfo.MaterialIndexData[i].Key;
		OutStaticMesh->IndexDataList.Add(MaterialName, TArray<uint32>());
		
		uint32 StartIndex = InObjInfo.MaterialIndexData[i].Value;
		uint32 NextStartIndex;
		if (i == InObjInfo.MaterialIndexData.Num() - 1)
			NextStartIndex = InObjInfo.VertexIndexList.Num();
		else
			NextStartIndex = InObjInfo.MaterialIndexData[i + 1].Value;
		
		for (uint32 Index = StartIndex; Index < NextStartIndex; Index++)
		{
			int32 PosIdx = InObjInfo.VertexIndexList[Index];
			int32 UVIdx = InObjInfo.UvIndexList[Index];
			int32 NormalIdx = InObjInfo.NormalIndexList[Index];
			// uint32 PosIdx = Indices[0];
			// uint32 UVIdx = Indices[1];
			// uint32 NormIdx = Indices[2];
			FObjVertex Vertex;
			Vertex.X = InObjInfo.VertexList[PosIdx].X;
			Vertex.Y = InObjInfo.VertexList[PosIdx].Y;
			Vertex.Z = InObjInfo.VertexList[PosIdx].Z;
			Vertex.U = InObjInfo.UVList[UVIdx].Key;
			Vertex.V = InObjInfo.UVList[UVIdx].Value;
			Vertex.NX = InObjInfo.NormalList[NormalIdx].X;
			Vertex.NY = InObjInfo.NormalList[NormalIdx].Y;
			Vertex.NZ = InObjInfo.NormalList[NormalIdx].Z;
			

			// TODO 리팩토링 필요
			// 중복 체크하는 시간이 너무 김
			//int32 findIndex = OutMesh.Vertices.Find(Vertex);
			//if (findIndex == -1)
			//{
			OutStaticMesh->Vertices.Add(Vertex);
			OutStaticMesh->IndexDataList[MaterialName].Add(OutStaticMesh->Vertices.Num() - 1);
			//}
			// else
			// {
			// 	OutMesh.Indices.Add(findIndex);
			// }
		}
	}

	for (const auto& MaterialData : InObjInfo.MaterialDataMap)
	{		
		OutStaticMesh->MaterialData.Add(MaterialData);
	}

	return OutStaticMesh;
}