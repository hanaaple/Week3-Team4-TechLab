#include "Core/Utils/ObjReader.h"

#include <fstream>
#include <sstream>

#include "Core/Container/String.h"

FStaticMesh ObjReader::Read(const FString& InFilePath)
{
	if (CheckFile(InFilePath))
	{
		FObjInfo ObjInfo = ReadRawData(InFilePath);
		FStaticMesh StaticMesh = CookRawData(ObjInfo);
		StaticMesh.PathFileName = *InFilePath;
		return StaticMesh;
	}
	return {};
}

bool ObjReader::CheckFile(const FString& InFilePath)
{
	std::ifstream f(*InFilePath);
	return f.good();
}

FObjInfo ObjReader::ReadRawData(const FString& InFilePath)
{
	std::ifstream File;
	if (!File.is_open())
	{
		File.open(*InFilePath);
	}
	FObjInfo ObjInfo;

	std::string Line; // std::string을 참조해야하므로, FString 대신 std::string 사용
	while (std::getline(File, Line))
	{
		std::stringstream TokenStream(Line);
		std::string Prefix;

		TokenStream >> Prefix;

		if (Prefix == "v")
		{
			FVector Pos;
			TokenStream >> Pos.X >> Pos.Y >> Pos.Z;
			ObjInfo.Vertices.Add(Pos);
		}
		else if (Prefix == "vt")
		{
			TArray<float> UV;
			float u, v;
			TokenStream >> u >> v;
			UV.Add(u);
			UV.Add(v);
			
			ObjInfo.UVs.Add(UV);
		}
		else if (Prefix == "vn")
		{
			FVector Normal;
			TokenStream >> Normal.X >> Normal.Y >> Normal.Z;
			ObjInfo.Normals.Add(Normal);
		}
		else if (Prefix == "f")
		{
			TArray<TArray<uint32>> Face;
			std::string VertexData;

			for (int i = 0; i < 3; ++i)
			{
				TArray<uint32> VertexIndex;
				TokenStream >> VertexData;
				std::replace(VertexData.begin(), VertexData.end(), '/', ' ');
				std::stringstream vss(VertexData);

				int posIdx, uvIdx, normIdx;
				vss >> posIdx >> uvIdx >> normIdx;
				VertexIndex.Add(posIdx);
				VertexIndex.Add(uvIdx);
				VertexIndex.Add(normIdx);
				Face.Add(VertexIndex);
			}
			ObjInfo.Faces.Add(Face);
		}
		//
		// if (Tokens.Num() <= 0)
		// 	continue;
		//
		// const std::string& Key = Tokens[0];
		//
		// if (Key == "v")
		// {
		// 	TArray<FVector> Vertex;
		// 	TokenStream >> 
		// 	Vertex.Add(std::stof(Tokens[1])); // Location X
		// 	Vertex.Add(std::stof(Tokens[2])); // Location Y
		// 	Vertex.Add(-std::stof(Tokens[3])); // Location Z
 	//
		// 	ObjInfo.Vertices.Add(Vertex);
		// }
		// else if (Key == "vn")
		// {
		// 	TArray<float> Normal;
		// 	Normal.Add(std::stof(Tokens[1])); // Normal X
		// 	Normal.Add(std::stof(Tokens[2])); // Normal Y
		// 	Normal.Add(-std::stof(Tokens[3])); // Normal Z
		// 	
		// 	ObjInfo.Normals.Add(Normal);
		// }
		// else if (Key == "vt")
		// {
		// 	TArray<float> UV;
		// 	UV.Add(std::stof(Tokens[1]));       // U
		// 	UV.Add(1.f - std::stof(Tokens[2])); // V; Obj 파일은 오른손 좌표계 기준이므로, 왼손 좌표계의 UV맵 좌표로 변경
 	//
		// 	ObjInfo.UVs.Add(UV);
		// }
		// else if (Key == "f")
		// {
		// 	TArray<TArray<uint32>> Face;
		// 	for (int i = 0; i < 3; ++i)
		// 	{
		// 		Face.Add(TArray<uint32>());
		// 		std::stringstream ss(Tokens[i + 1]);
		// 		std::string Val;
		// 		int Cnt = 0;
		// 		while (std::getline(ss, Val, '/'))
		// 		{
		// 			Face[i].Add(std::stoi(Val) - 1); // 인덱스는 1부터 시작하므로, 1 감소
		// 			++Cnt;
		// 		}
		// 	}
		// 	ObjInfo.Faces.Add(Face);
		//}
	}
	File.close();

	return ObjInfo;
}

FStaticMesh ObjReader::CookRawData(FObjInfo ObjInfo)
{
	FStaticMesh OutMesh;

	for (int Section = 0; Section < ObjInfo.Faces.Num(); ++Section)
	{
		for (int FaceVertexIndex = 0; FaceVertexIndex < ObjInfo.Faces[Section].Num(); ++FaceVertexIndex)
		{
			const TArray<uint32>& Indices = ObjInfo.Faces[Section][FaceVertexIndex];

			uint32 PosIdx = Indices[0];
			uint32 UVIdx = Indices[1];
			uint32 NormIdx = Indices[2];
			
			FNormalVertex Vertex;
			Vertex.X = ObjInfo.Vertices[PosIdx - 1].X;
			Vertex.Y = ObjInfo.Vertices[PosIdx - 1].Y;
			Vertex.Z = ObjInfo.Vertices[PosIdx - 1].Z;
			
			Vertex.U = ObjInfo.UVs[UVIdx - 1][0];
			Vertex.V = ObjInfo.UVs[UVIdx - 1][1]; // Assuming UVs are 1D for now
			Vertex.NX = ObjInfo.Normals[NormIdx - 1].X;
			Vertex.NY = ObjInfo.Normals[NormIdx - 1].Y;
			Vertex.NZ = ObjInfo.Normals[NormIdx - 1].Z;
			
			//int32 findIndex = OutMesh.Vertices.Find(Vertex);
			// 중복은?
			//if (findIndex == -1)
			{
				OutMesh.Vertices.Add(Vertex);
				OutMesh.Indices.Add(OutMesh.Vertices.Num() - 1);
			}
			// else
			// {
			// 	OutMesh.Indices.Add(findIndex);
			// }
		}
		
	}

	return OutMesh;
}