#pragma once
#include "Texture.h"
#include "Resource/Resource.h"
#include "Core/Container/String.h"
#include "DirectResource/ShaderResourceBinding.h"
#include "Resource/DirectResource/Vertexbuffer.h"


class FDepthStencilState;
class FRasterizer;
class FBlendState;

class FMaterial : public FResource<FMaterial>
{
public:
	FMaterial();
	virtual ~FMaterial() override;

	static std::shared_ptr<FMaterial> Create(const FString& InName)
	{
		std::shared_ptr<FMaterial> NewRes = CreateRes(InName);
		return NewRes;
	}

	void VertexShader();
	void Rasterizer();
	void PixelShader();
	void Blend();
	void DepthStencil();
	void Texture();
	void UpdateConstantBuffer();

	void SetVertexShader(const FString& InValue);
	void SetRasterizer(const FString& InValue);
	void SetPixelShader(const FString& InValue);
	void SetBlendState(const FString& InValue);
	void SetDepthState(const FString& InValue);
	void SetTexture(const FString& InValue, uint8 InSlot);
	void SetConstantBuffer(const FString& InValue, const void* CPUDataPtr, int DataSize, int BindPoint, bool bIsUseVertexShader, bool bIsUsePixelShader);
	
	std::shared_ptr<class FVertexShader> GetVertexShader()
	{
		return VertexShaderPtr;
	}

	std::shared_ptr<class FPixelShader> GetPixelShader()
	{
		return PixelShaderPtr;
	}
	
	void Setting();
	
private:
	std::shared_ptr<FVertexShader> VertexShaderPtr = nullptr;
	std::shared_ptr<FPixelShader> PixelShaderPtr = nullptr;
	std::shared_ptr<FRasterizer> RasterizerPtr = nullptr;
	std::shared_ptr<FBlendState> BlendStatePtr = nullptr;
	std::shared_ptr<FDepthStencilState> DepthStencilPtr = nullptr;
	TMap<uint8, std::shared_ptr<FTexture>> TexturePtrMap;
	TArray<std::shared_ptr<FConstantBufferBinding>> ConstantBufferBindingPtrArray;
};



