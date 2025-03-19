#pragma once
#include"Object/Actor/Actor.h"
#include "wrl.h"

class AWorldGrid : public AActor
{
	DECLARE_CLASS(AWorldGrid, AActor)

public:
	AWorldGrid();
	~AWorldGrid();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void LateTick(float DeltaTime) override;

private:
	float GridSize;
	float GridSpacing;
	FVector4 GridColor;
	bool bCenterGrid;


private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> LineVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> LineIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> LineConstantBuffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> LineVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> LinePixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> LineInputLayout;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> LineRasterizerState;
};

