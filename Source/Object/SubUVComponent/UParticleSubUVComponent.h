#pragma once
#include "Core/HAL/PlatformType.h"
#include <d3dcompiler.h>
#include "Object/USceneComponent.h"
#include "Resource/DirectResource/Vertexbuffer.h"
#include "Resource/DirectResource/IndexBuffer.h"
#include "Resource/DirectResource/ConstantBuffer.h"
#include "Resource/DirectResource/VertexShader.h"
#include "Resource/DirectResource/PixelShader.h"
#include "Resource/DirectResource/InputLayout.h"
#include "Resource/DirectResource/BlendState.h"
#include "Resource/DirectResource/DepthStencilState.h"
#include "Resource/DirectResource/Rasterizer.h"
#include "Resource/RenderResourceCollection.h"
#include "Resource/DirectResource/ShaderResourceBinding.h"


struct FSubUVTextureInfo
{
	ID3D11ShaderResourceView* TextureSRV;
	uint32 NumRows;
	uint32 NumColumns;
	uint32 TotalFrames;
	float FrameWidth;
	float FrameHeight;
};

struct alignas(16) FSubUVVertexConstantsData
{
	FMatrix MVP;
};

struct alignas(16) FSubUVPixelConstantsData
{

};

class UParticleSubUVComponent : public USceneComponent
{
	DECLARE_CLASS(UParticleSubUVComponent, USceneComponent)

public:
	UParticleSubUVComponent();
	//UParticleSubUVComponent(FSubUVTextureInfo &Texture);

	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void CalculateModelMatrix(FMatrix& OutMatrix);
	void Render();

	// SubUV 애니메이션 관련 메서드
	void Play();
	void Pause();
	void Stop();
	void SetPlayRate(float NewRate);
	void SetLooping(bool bShouldLoop);

	// 현재 프레임의 UV 좌표 반환
	FVector4 GetCurrentFrameUV() const;
	FSubUVVertexConstantsData& GetVertexConstantsData() { return VertexConstantsData; }
	FSubUVPixelConstantsData& GetPixelConstantsData() { return PixelConstantsData; }
	FRenderResourceCollection& GetRenderResourceCollection() { return RenderResourceCollection; }

private:
	bool bLoop;
	bool bIsPlaying;
	float PlayRate;
	float CurrentTime;
	uint32 CurrentFrame;
	uint32 TotalFrames;

	FSubUVTextureInfo TextureInfo;
	FSubUVVertexConstantsData VertexConstantsData;
	FSubUVPixelConstantsData PixelConstantsData;
	FRenderResourceCollection RenderResourceCollection;

	std::shared_ptr<class FVertexBuffer> VertexBuffer = nullptr;
	std::shared_ptr<class FIndexBuffer> IndexBuffer = nullptr;
	std::shared_ptr<class FConstantBuffer> ConstantBuffer = nullptr;
	ID3D11InputLayout* InputLayout = nullptr;
	D3D_PRIMITIVE_TOPOLOGY Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	//class std::shared_ptr<class FPixelShader> PixelShader = nullptr;
	//class std::shared_ptr<class FVertexShader> VertexShader = nullptr;
	ID3D11VertexShader* VertexShader = nullptr;
	ID3D11PixelShader* PixelShader = nullptr;
	class std::shared_ptr<class FBlendState> BlendState = nullptr;
	class std::shared_ptr<class FDepthStencilState> DepthStencilStat = nullptr;
	class std::shared_ptr<class FRasterizer> Rasterizer = nullptr;
};
