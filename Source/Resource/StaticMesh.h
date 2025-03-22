#pragma once

#define _TCHAR_DEFINED
#include <d3d11.h>

#include "Core/Engine.h"
#include "Resource/Resource.h"
#include "Core/Container/String.h"
#include "Core/Container/Array.h"
#include "Core/UObject/Object.h"
#include "Core/UObject/ObjectMacros.h"
#include "DirectResource/Shader.h"
#include "Resource/DirectResource/Vertexbuffer.h"
#include "Resource/DirectResource/IndexBuffer.h"
#include "Debug/DebugConsole.h"

class UStaticMesh : public FResource<UStaticMesh>, public UObject
{
	DECLARE_CLASS(UStaticMesh, UObject)
public:
	UStaticMesh() = default;

	static std::shared_ptr<UStaticMesh> Create(const FString& InName, D3D_PRIMITIVE_TOPOLOGY Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		return Create(InName, InName, InName, Topology);
	}

	static std::shared_ptr<UStaticMesh> Create(
		const FString& InName, const FString& VertexName, const FString& IndexName
		, D3D_PRIMITIVE_TOPOLOGY Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	)
	{
		std::shared_ptr<UStaticMesh> Res = CreateUObjectRes(InName);
		
		Res->VertexBuffer = FVertexBuffer::Find(VertexName);
		Res->IndexBuffer = FIndexBuffer::Find(IndexName);
		Res->Topology = Topology;

		if (nullptr == Res->VertexBuffer
			|| nullptr == Res->IndexBuffer)
		{
			MsgBoxAssert("매쉬를 만드는데 실패했습니다.");
		}

		UEngine::Get().GObjects.Add(Res->GetUUID(), Res);
		
		return Res;
	}

	static std::shared_ptr<UStaticMesh> Create(
		const FString& InName, const std::shared_ptr<FVertexBuffer>& InVertex, const std::shared_ptr<FIndexBuffer>& InIndex,
		D3D_PRIMITIVE_TOPOLOGY Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	)
	{
		std::shared_ptr<UStaticMesh> Res = CreateUObjectRes(InName);
		
		Res->VertexBuffer = InVertex;
		Res->IndexBuffer = InIndex;
		Res->Topology = Topology;

		if (nullptr == Res->VertexBuffer
			|| nullptr == Res->IndexBuffer)
		{
			MsgBoxAssert("매쉬를 만드는데 실패했습니다.");
		}
		
		UEngine::Get().GObjects.Add(Res->GetUUID(), Res);
		return Res;
	}

	void SetTopology(D3D11_PRIMITIVE_TOPOLOGY InTopology)
	{
		Topology = InTopology;
	}

	void Setting();
	void Draw();

	std::shared_ptr<FVertexBuffer> GetVertexBuffer()
	{
		return VertexBuffer;
	}

	std::shared_ptr<FIndexBuffer> GetIndexBuffer()
	{
		return IndexBuffer;
	}
	
private:
	std::shared_ptr<FVertexBuffer> VertexBuffer = nullptr;
	std::shared_ptr<FIndexBuffer> IndexBuffer = nullptr;
	D3D_PRIMITIVE_TOPOLOGY Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};
