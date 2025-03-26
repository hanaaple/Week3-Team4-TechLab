#include "StaticMeshCreator.h"

#include "StaticMesh.h"
#include "Core/Utils/ObjReader.h"
#include "Object/Actor/Camera.h"
#include "Object/World/World.h"
#include "Resource/DirectResource/Vertexbuffer.h"
#include "Static/FEditorManager.h"


void FStaticMeshCreator::CreateMesh(const FString& FilePath, const FString& MeshName)
{
	FStaticMesh* StaticMeshAsset = ObjReader::Read(FilePath);
	StaticMeshAsset->Name = MeshName;
	FVertexBuffer::Create(MeshName, StaticMeshAsset->Vertices);

	UStaticMesh* StaticMesh = UStaticMesh::Create(MeshName, true).get();
	StaticMesh->SetStaticMeshAsset(StaticMeshAsset);
			
	for (FObjMaterialInfo& MaterialInfo : StaticMeshAsset->MaterialData)
	{
		FString MaterialName = MaterialInfo.MaterialName;
		if (!StaticMeshAsset->IndexDataList.Contains(MaterialName))
			continue;
		FString IndexBufferName = MeshName + MaterialName;

		// 머티리얼 이름 - 머티리얼 이름
		// 인덱스 버퍼 이름 - MeshName + MaterialName
				
		FIndexBuffer::Create(IndexBufferName, StaticMeshAsset->IndexDataList[MaterialInfo.MaterialName]);


		std::shared_ptr<FMaterial> Mat = FMaterial::Create(MaterialName);
		Mat->SetRasterizer("DefaultRasterizer");
		Mat->SetBlendState("DefaultBlendState");
		Mat->SetDepthState("DefaultDepthStencilState");
		Mat->SetVertexShader("StaticMesh_VS");
		Mat->SetPixelShader("StaticMesh_PS");
				
		// Create라는게 있다. 나중에 확인해봐라.
		// FTexture::Create(MaterialInfo.DiffuseMap, textureDesc);
		if (!MaterialInfo.AmbientMap.IsEmpty())
		{
			auto Texture = FTexture::Load(MaterialInfo.AmbientMap, MaterialInfo.AmbientMap);
			Texture->CreateShaderResourceView();
		}
		if (!MaterialInfo.DiffuseMap.IsEmpty())
		{
			auto Texture = FTexture::Load(MaterialInfo.DiffuseMap, MaterialInfo.DiffuseMap);
			Texture->CreateShaderResourceView();
		}
		if (!MaterialInfo.SpecularMap.IsEmpty())
		{
			auto Texture =	FTexture::Load(MaterialInfo.SpecularMap, MaterialInfo.SpecularMap);
			Texture->CreateShaderResourceView();
		}
		if (!MaterialInfo.SpecularExponentMap.IsEmpty())
		{
			auto Texture =	FTexture::Load(MaterialInfo.SpecularExponentMap, MaterialInfo.SpecularExponentMap);
			Texture->CreateShaderResourceView();
		}
		if (!MaterialInfo.DissolveMap.IsEmpty())
		{
			auto Texture =	FTexture::Load(MaterialInfo.DissolveMap, MaterialInfo.DissolveMap);
			Texture->CreateShaderResourceView();
		}
		if (!MaterialInfo.BumpMap.IsEmpty())
		{
			auto Texture =	FTexture::Load(MaterialInfo.BumpMap, MaterialInfo.BumpMap);
			Texture->CreateShaderResourceView();
		}

		struct alignas(16) FStaticMeshConstantInfo
		{	
			float SpecularExponent;			// Ns
			FVector Ambient;				// Ka
			FVector Diffuse;				// Kd
			FVector Specular;				// Ks
			FVector Emissive;				// Ke

			float OpticalDensity;			// Ni
			float Dissolve;					// d     Dissolve / Transparency
			int32 Illumination;			// illum
		};

		FStaticMeshConstantInfo* StaticMeshConstantInfo = new FStaticMeshConstantInfo();
		StaticMeshConstantInfo->SpecularExponent = MaterialInfo.SpecularExponent;
		StaticMeshConstantInfo->Ambient = MaterialInfo.Ambient;
		StaticMeshConstantInfo->Diffuse = MaterialInfo.Diffuse;
		StaticMeshConstantInfo->Specular = MaterialInfo.Specular;
		StaticMeshConstantInfo->Emissive = MaterialInfo.Emissive;
		StaticMeshConstantInfo->OpticalDensity = MaterialInfo.OpticalDensity;
		StaticMeshConstantInfo->Dissolve = MaterialInfo.Dissolve;
		StaticMeshConstantInfo->Illumination = MaterialInfo.Illumination;
				
		Mat->AddConstantBuffer(MaterialName, StaticMeshConstantInfo, sizeof(FStaticMeshConstantInfo), 2, true, true);

		Mat->AddConstantBuffer(MaterialName + TEXT("Camera"), &StaticMesh->GetCameraConstantData(), sizeof(FCameraConstantData), 3, true, true);
		
		Mat->SetTexture(MaterialInfo.AmbientMap, 1);
		Mat->SetTexture(MaterialInfo.DiffuseMap, 2);
		Mat->SetTexture(MaterialInfo.SpecularMap, 3);
		Mat->SetTexture(MaterialInfo.SpecularExponentMap, 4);
		Mat->SetTexture(MaterialInfo.DissolveMap, 5);
		Mat->SetTexture(MaterialInfo.BumpMap, 6);

		StaticMesh->AddMaterial(Mat);
	}
}
