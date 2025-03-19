#include "UDebugDrawManager.h"
#include "Core/Engine.h"
#include "Object/World/World.h"
#include "Object/Actor/Camera.h"
#include "Resource/DirectResource/Vertexbuffer.h"
#include "Resource/DirectResource/IndexBuffer.h"
#include "Resource/DirectResource/PixelShader.h"
#include "Resource/DirectResource/VertexShader.h"
#include "Resource/DirectResource/InputLayout.h"
#include "Resource/DirectResource/BlendState.h"
#include "Resource/DirectResource/Rasterizer.h"
#include "Resource/DirectResource/DepthStencilState.h"
#include "Resource/DirectResource/ConstantBuffer.h"
#include "Resource/Mesh.h"
#include "Resource/Material.h"
#include "Core/Math/BoxSphereBounds.h"

using namespace std;

UDebugDrawManager::UDebugDrawManager()
{
}

UDebugDrawManager::~UDebugDrawManager()
{
	VertexBuffer.Empty();
	IndexBuffer.Empty();
}

void UDebugDrawManager::Initialize()
{
	FVertexBuffer::Create(TEXT("DebugVertexBuffer"), VertexBuffer, true);
	FIndexBuffer::Create(TEXT("DebugIndexBuffer"), IndexBuffer, true);
	std::shared_ptr<FMesh> Mesh = FMesh::Create("DebugBatchMesh", "DebugVertexBuffer", "DebugIndexBuffer", D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE; // 라인은 양면을 볼 수 있도록 컬링 없음
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.ScissorEnable = FALSE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.AntialiasedLineEnable = TRUE; // 안티앨리어싱된 라인 활성화

	FRasterizer::Create(TEXT("DebugRasterizerState"), rasterizerDesc);

	std::shared_ptr<FMaterial> Material = FMaterial::Create("DebugBatchMaterial");
	Material->SetVertexShader(TEXT("ShaderLine_VS"));
	Material->SetPixelShader(TEXT("ShaderLine_PS"));
	Material->SetRasterizer(TEXT("DebugRasterizerState"));

	RenderResourceCollection.SetConstantBufferBinding(TEXT("DebugConstantInfo"), &DebugConstantInfo, 1, true, false);

	RenderResourceCollection.SetMesh(Mesh);
	RenderResourceCollection.SetMaterial(Material);
}

void UDebugDrawManager::DrawBoxBrackets(const FBox InActor, const FTransform& InTransform, const FVector4& Color, float LifeTime)
{
	FVector Min = InActor.Min;
	FVector Max = InActor.Max;
	FVector Vertices[8];
	Vertices[0] = FVector(Min.X, Min.Y, Min.Z);
	Vertices[1] = FVector(Min.X, Min.Y, Max.Z);
	Vertices[2] = FVector(Min.X, Max.Y, Min.Z);
	Vertices[3] = FVector(Min.X, Max.Y, Max.Z);
	Vertices[4] = FVector(Max.X, Min.Y, Min.Z);
	Vertices[5] = FVector(Max.X, Min.Y, Max.Z);
	Vertices[6] = FVector(Max.X, Max.Y, Min.Z);
	Vertices[7] = FVector(Max.X, Max.Y, Max.Z);
	for (int i = 0; i < 8; i++)
	{
		Vertices[i] = InTransform.TransformPosition(Vertices[i]);
	}
	DrawLine(Vertices[0], Vertices[1], Color, LifeTime);
	DrawLine(Vertices[1], Vertices[3], Color, LifeTime);
	DrawLine(Vertices[3], Vertices[2], Color, LifeTime);
	DrawLine(Vertices[2], Vertices[0], Color, LifeTime);
	DrawLine(Vertices[4], Vertices[5], Color, LifeTime);
	DrawLine(Vertices[5], Vertices[7], Color, LifeTime);
	DrawLine(Vertices[7], Vertices[6], Color, LifeTime);
	DrawLine(Vertices[6], Vertices[4], Color, LifeTime);
	DrawLine(Vertices[0], Vertices[4], Color, LifeTime);
	DrawLine(Vertices[1], Vertices[5], Color, LifeTime);
	DrawLine(Vertices[2], Vertices[6], Color, LifeTime);
	DrawLine(Vertices[3], Vertices[7], Color, LifeTime);
}

void UDebugDrawManager::DrawBox(const FVector& Origin, const FVector& BoxExtent, const FVector4& Color, float LifeTime)
{

}

void UDebugDrawManager::DrawSphere(const FVector& Center, float Radius, const FVector4& Color, float LifeTime)
{
}

void UDebugDrawManager::DrawBoxSphereBounds(const FVector& Origin, const FVector& BoxExtent, float SphereRadius, const FVector4& Color, float LifeTime)
{

}

void UDebugDrawManager::DrawLine(const FVector& Start, const FVector& End, const FVector4& Color, float LifeTime)
{
	uint32 Index = VertexBuffer.Num();
	
	VertexBuffer.Add({ Start, Color });
	VertexBuffer.Add({ End, Color });

	IndexBuffer.Add(Index);
	IndexBuffer.Add(Index + 1);
}

void UDebugDrawManager::DrawPoint(const FVector& Point, const FVector4& Color, float LifeTime)
{
}

void UDebugDrawManager::DrawDebug()
{
	if (VertexBuffer.Num() == 0)
	{
		return;
	}

	DebugConstantInfo.ViewProjectionMatrix = FMatrix::Transpose(UEngine::Get().GetWorld()->GetCamera()->GetViewProjectionMatrix());
	RenderResourceCollection.Render();
}

void UDebugDrawManager::ClearDebug()
{
	VertexBuffer.Empty();
	IndexBuffer.Empty();
}
