#pragma once
#include "Core/AbstractClass/Singleton.h"
#include "Primitive/PrimitiveVertices.h"
#include "Core/Container/Array.h"
#include "Resource/RenderResourceCollection.h"
#include "wrl.h"
#include "Core/Math/Matrix.h"

struct FVector;
struct FVector4;
struct FBox;

struct alignas(16) FDebugConstantInfo
{
	FMatrix ViewProjectionMatrix;
};

class UDebugDrawManager : public TSingleton<UDebugDrawManager>
{
public:
	UDebugDrawManager();
	~UDebugDrawManager();

	void Initialize();
	void Render();

	void DrawBoxBrackets(FBox InActor, const FTransform& LocalToWorld, const FVector4& Color, float LifeTime = 1.0f);
	void DrawBox(const FVector& InMin, const FVector& InMax, const FVector4& Color, float LifeTime = 1.0f);
	void DrawSphere(const FVector& Center, float Radius, const FVector4& Color, float LifeTime = 1.0f);
	void DrawBoxSphereBounds(const FVector& Origin, const FVector& BoxExtent, float SphereRadius, const FVector4& Color, float LifeTime = 1.0f);
	void DrawLine(const FVector& Start, const FVector& End, const FVector4& Color, float LifeTime = 1.0f);
	void DrawPoint(const FVector& Point, const FVector4& Color, float LifeTime = 1.0f);
	//void DrawText(const FVector& Location, const FString& Text, const FColor& Color, float LifeTime = 0.0f);
	void ClearDebug();

public:
	FRenderResourceCollection& GetRenderResourceCollection() { return RenderResourceCollection; }
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> GetLineRasterizerState() { return LineRasterizerState; }
private:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> LineRasterizerState = nullptr; // 종종 다른 컬링 방식 필요
	FDebugConstantInfo DebugConstantInfo;
	FRenderResourceCollection RenderResourceCollection;
public:
	TArray<FLineVertexSimple>* GetDebugLineVertexBuffer() { return &VertexBuffer; }
	TArray<uint32>* GetDebugLineIndexBuffer() { return &IndexBuffer; }

private:
	TArray<FLineVertexSimple> VertexBuffer;
	TArray<uint32> IndexBuffer;
};

