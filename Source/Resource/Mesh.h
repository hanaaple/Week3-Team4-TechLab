#pragma once

#define _TCHAR_DEFINED
#include <d3d11.h>

#include "Resource/Resource.h"
#include "Core/Container/String.h"
#include "Core/Container/Array.h"
#include "DirectResource/Shader.h"
#include "Resource/DirectResource/Vertexbuffer.h"
#include "Resource/DirectResource/IndexBuffer.h"
#include "Debug/DebugConsole.h"

class FMesh : public FResource<FMesh>
{
public:

	FMesh();
	~FMesh();

	static std::shared_ptr<FMesh> Create(const FString& _Name, D3D_PRIMITIVE_TOPOLOGY Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		return Create(_Name, _Name, _Name, Topology);
	}

	static std::shared_ptr<FMesh> Create(const FString& _Name, const FString& _VtxName, const FString& _IdxName
		, D3D_PRIMITIVE_TOPOLOGY Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		std::shared_ptr<FMesh> Res = CreateRes(_Name);
		Res->VertexBuffer = FVertexBuffer::Find(_VtxName);
		Res->IndexBuffer = FIndexBuffer::Find(_IdxName);
		Res->Topology = Topology;

		if (nullptr == Res->VertexBuffer
			|| nullptr == Res->IndexBuffer)
		{
			MsgBoxAssert("매쉬를 만드는데 실패했습니다.");
		}

		return Res;
	}

	static std::shared_ptr<FMesh> Create(const FString& _Name, std::shared_ptr<class FVertexBuffer> _Vtx, std::shared_ptr<FIndexBuffer> _Idx,
		D3D_PRIMITIVE_TOPOLOGY Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		std::shared_ptr<FMesh> Res = CreateRes(_Name);
		Res->VertexBuffer = _Vtx;
		Res->IndexBuffer = _Idx;
		Res->Topology = Topology;

		if (nullptr == Res->VertexBuffer
			|| nullptr == Res->IndexBuffer)
		{
			MsgBoxAssert("매쉬를 만드는데 실패했습니다.");
		}

		return Res;
	}

	void SetTOPOLOGY(D3D11_PRIMITIVE_TOPOLOGY _TOPOLOGY)
	{
		Topology = _TOPOLOGY;
	}

	void Setting();
	void Draw();

	std::shared_ptr<class FVertexBuffer> GetVertexBuffer()
	{
		return VertexBuffer;
	}

	std::shared_ptr<class FIndexBuffer> GetIndexBuffer()
	{
		return IndexBuffer;
	}
	
private:
	std::shared_ptr<class FVertexBuffer> VertexBuffer = nullptr;
	std::shared_ptr<class FIndexBuffer> IndexBuffer = nullptr;
	D3D_PRIMITIVE_TOPOLOGY Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};
