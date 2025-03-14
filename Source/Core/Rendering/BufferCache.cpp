#include "BufferCache.h"
#include "Core/Engine.h"
#include "Primitive/PrimitiveVertices.h"
#include "Primitive/UGeometryGenerator.h"
#include "Resource/Vertexbuffer.h"

FBufferCache::FBufferCache()
{
}

FBufferCache::~FBufferCache()
{
}

void FBufferCache::Init()
{

}

const BufferInfo FBufferCache::GetBufferInfo(EPrimitiveType Type)
{
	if (!Cache.Contains(Type))
	{
		
		//여기서 버텍스 버퍼 생성한다
		auto bufferInfo = CreateBufferInfo(Type);
		Cache.Add( Type , bufferInfo);
	}

	return *Cache.Find(Type);
}

BufferInfo FBufferCache::CreateBufferInfo(EPrimitiveType Type)
{
	ID3D11Buffer* VertexBuffer = nullptr;
	ID3D11Buffer* IndexBuffer = nullptr;
	int VertexSize = 0;
	int IndexSize = 0;
	D3D_PRIMITIVE_TOPOLOGY Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	switch (Type)
	{
	case EPrimitiveType::EPT_Line:
	{
		FVertexSimple LineVertices[2] =
		{
			{ -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f }
		};

		uint32 LineIndices[2] =
		{
			0, 1
		};
		
		VertexSize = std::size(LineVertices);
		IndexSize = std::size(LineIndices);
		VertexBuffer = UEngine::Get().GetRenderer()->CreateVertexBuffer(LineVertices, sizeof(FVertexSimple) * VertexSize);
		IndexBuffer = UEngine::Get().GetRenderer()->CreateIndexBuffer(LineIndices, sizeof(uint32) * IndexSize);
		Topology = D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
		break;
	}
	case EPrimitiveType::EPT_Triangle:
	{
		FVertexSimple TriangleVertices[3] =
		{
			{  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f },
			{  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f },
			{  0.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f } 
		};

		uint32 TriangleIndices[3] =
		{
			0, 1, 2
		};

		VertexSize = std::size(TriangleVertices);
		IndexSize = std::size(TriangleIndices);
		VertexBuffer = UEngine::Get().GetRenderer()->CreateVertexBuffer(TriangleVertices, sizeof(FVertexSimple) * VertexSize);
		IndexBuffer = UEngine::Get().GetRenderer()->CreateIndexBuffer(TriangleIndices, sizeof(uint32) * IndexSize);
		break;
	}
	case EPrimitiveType::EPT_Cube:
	{
		FGeometryData GeometryData = CreateCubeGeometry();

		VertexSize = GeometryData.Vertices.Num();
		IndexSize = GeometryData.Indices.Num();
		VertexBuffer = UEngine::Get().GetRenderer()->CreateVertexBuffer(GeometryData.Vertices.GetData(), sizeof(FVertexSimple) * VertexSize);
		IndexBuffer = UEngine::Get().GetRenderer()->CreateIndexBuffer(GeometryData.Indices.GetData(), sizeof(uint32) * IndexSize);

		FVertexBuffer::Create(FString("Cube"), GeometryData.Vertices);

			
		break;
	}
	case EPrimitiveType::EPT_Sphere:
	{
		FGeometryData GeometryData = CreateSphereGeometry();

		VertexSize = GeometryData.Vertices.Num();
		IndexSize = GeometryData.Indices.Num();
		VertexBuffer = UEngine::Get().GetRenderer()->CreateVertexBuffer(GeometryData.Vertices.GetData(), sizeof(FVertexSimple) * VertexSize);
		IndexBuffer = UEngine::Get().GetRenderer()->CreateIndexBuffer(GeometryData.Indices.GetData(), sizeof(uint32) * IndexSize);
		break;
	}
	case EPrimitiveType::EPT_Cylinder:
	{
		FGeometryData GeometryData = CreateCylinderGeometry();

		VertexSize = GeometryData.Vertices.Num();
		IndexSize = GeometryData.Indices.Num();
		VertexBuffer = UEngine::Get().GetRenderer()->CreateVertexBuffer(GeometryData.Vertices.GetData(), sizeof(FVertexSimple) * VertexSize);
		IndexBuffer = UEngine::Get().GetRenderer()->CreateIndexBuffer(GeometryData.Indices.GetData(), sizeof(uint32) * IndexSize);
		break;
	}
	case EPrimitiveType::EPT_Cone:
	{
		FGeometryData GeometryData = CreateConeGeometry();
		
		VertexSize = GeometryData.Vertices.Num();
		IndexSize = GeometryData.Indices.Num();
		VertexBuffer = UEngine::Get().GetRenderer()->CreateVertexBuffer(GeometryData.Vertices.GetData(), sizeof(FVertexSimple) * VertexSize);
		IndexBuffer = UEngine::Get().GetRenderer()->CreateIndexBuffer(GeometryData.Indices.GetData(), sizeof(uint32) * IndexSize);
		break;
	}
	}

	return BufferInfo(VertexBuffer, VertexSize, IndexBuffer, IndexSize, Topology);
}


FGeometryData FBufferCache::CreateCubeGeometry()
{
	TArray<FVertexSimple> vertices;
	TArray<uint32> indices;
	int size = 1.f;

	UGeometryGenerator::CreateCube(size, &vertices, &indices);

	return { vertices, indices };
}

FGeometryData FBufferCache::CreateSphereGeometry()
{
	TArray<FVertexSimple> vertices;
	TArray<uint32> indices;
	int slices = 16;
	int stacks = 16;
	float radius = 1.f;
	float height = 1.f;

	UGeometryGenerator::CreateSphere(radius, slices, stacks, &vertices, &indices);

	return { vertices, indices };
}

FGeometryData FBufferCache::CreateConeGeometry()
{
	TArray<FVertexSimple> vertices;
	TArray<uint32> indices;
	int slices = 36;
	int stacks = 6;
	float radius = 1.f;
	float height = 1.f;

	UGeometryGenerator::CreateCone(radius, height, slices, stacks, &vertices, &indices);

	return { vertices, indices };
}

FGeometryData FBufferCache::CreateCylinderGeometry()
{
	TArray<FVertexSimple> vertices;
	TArray<uint32> indices;
	int slices = 36;
	int stacks = 36;
	float bRadius = .2f;
	float tRdius = .2f;
	float height = 1.f;

	UGeometryGenerator::CreateCylinder(bRadius, tRdius, height, slices, stacks, &vertices , &indices);

	return {vertices, indices};
}
