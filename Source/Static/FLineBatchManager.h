#pragma once

#include "Object/Actor/Actor.h"
#include "Primitive/PrimitiveVertices.h"
#include "Core/Container/Array.h"

struct FLineConstantInfo
{
	FMatrix ViewProjectionMatrix;
};


class FLineBatchManager : public TSingleton<FLineBatchManager>
{
public:
	// 라인 추가 함수
	void AddLine(const FVector& Start, const FVector& End, const  FVector4& Color = FVector4{1.0f,1.0f,1.0f,1.0f}, float Thickness = 1.0f);

	// 배치 처리 함수
	void Flush();

	void DrawWorldGrid(float GridSize, float GridSpacing, const FVector4& GridColor = FVector4{1.0f,1.0f,1.0f,1.0f }, bool bCenterGrid = true);


private:
	TArray<FLineVertexSimple> VertexBuffer;
	TArray<uint32> IndexBuffer;


	// 최대 용량
	UINT MaxVerticesPerBatch = 10000;
	UINT MaxIndicesPerBatch = MaxVerticesPerBatch * 2;


	// 선택적 디버그/시각화 플래그
	bool bShowDebugLines = true;
	bool bShowWorldGrid = true;
	float GridSize = 100.0f;
	float GridSpacing = 10.0f;
	FVector4 GridColor = { 0.3f, 0.3f, 0.3f, 1.0f };


	//추후 렌더러로 분리

public:
	void Create();
	void Render();

private:
	// 
	// GPU 렌더링을 위한 버퍼
	ID3D11Buffer* LineVertexBuffer = nullptr;
	ID3D11Buffer* LineIndexBuffer = nullptr;
	ID3D11Buffer* LineConstantBuffer = nullptr;                 // 쉐이더에 데이터를 전달하기 위한 상수 버퍼


	// 라인 렌더링 전용 쉐이더 리소스
	ID3D11VertexShader* LineVertexShader = nullptr;
	ID3D11PixelShader* LinePixelShader = nullptr;
	ID3D11InputLayout* LineInputLayout = nullptr;

	// 라인 렌더링 전용 상태
	D3D11_PRIMITIVE_TOPOLOGY LinePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	// 라인 렌더링을 위한 특수 상태
	ID3D11RasterizerState* LineRasterizerState = nullptr; // 종종 다른 컬링 방식 필요
};


