#pragma once

#define _TCHAR_DEFINED
#include <d3d11.h>

#include <wrl/client.h>
#include <unordered_map>
#include "Primitive/PrimitiveVertices.h"
#include "Core/Container/Array.h"
#include "Core/Container/Map.h"

struct BufferInfo
{
public:
	BufferInfo() = default;
	BufferInfo(ID3D11Buffer* InVertexBuffer, int VertexBufferSize, ID3D11Buffer* InIndexBuffer, int IndexBufferSize, D3D_PRIMITIVE_TOPOLOGY InTopology)
	{
		VertexBuffer = InVertexBuffer;
		VertexSize = VertexBufferSize;
		IndexBuffer = InIndexBuffer;
		IndexSize = IndexBufferSize;
		Topology = InTopology;
	}

	ID3D11Buffer* GetVertexBuffer() const { return VertexBuffer.Get(); }
	ID3D11Buffer* GetIndexBuffer() const { return IndexBuffer.Get(); }
	int GetVertexSize() const { return VertexSize; }
	int GetIndexSize() const { return IndexSize; }
	D3D_PRIMITIVE_TOPOLOGY GetTopology() const { return Topology; }

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;
	D3D_PRIMITIVE_TOPOLOGY Topology;
	int VertexSize;
	int IndexSize;
};

class FBufferCache
{
private:
	TMap<EPrimitiveType, BufferInfo> Cache;

public:
	FBufferCache();
	~FBufferCache();

	void Init();
	const BufferInfo GetBufferInfo(EPrimitiveType Type);

public:
	FGeometryData CreateArrowGeometry();
	FGeometryData CreateCubeGeometry();
	FGeometryData CreateSphereGeometry();
	FGeometryData CreateConeGeometry();
	FGeometryData CreateCylinderGeometry();

private :
	BufferInfo CreateBufferInfo(EPrimitiveType Type);
};

