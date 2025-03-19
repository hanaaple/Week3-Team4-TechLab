#pragma once
#define _TCHAR_DEFINED

#include <d3d11.h>
#include "Resource/Resource.h"
#include "Core/Container/String.h"
#include "Resource/DirectResource/Vertexbuffer.h"
#include "Debug/DebugConsole.h"

class FMaterial : public FResource<FMaterial>
{
public:
	FMaterial();
	~FMaterial();

	static std::shared_ptr<class FMaterial> Create(FString _Name)
	{
		std::shared_ptr<class FMaterial> NewRes = CreateRes(_Name);
		return NewRes;
	}

	void VertexShader();
	void Rasterizer();
	void PixelShader();
	void Blend();
	void DepthStencil();


	void SetVertexShader(const FString& _Value);
	void SetRasterizer(const FString& _Value);
	void SetPixelShader(const FString& _Value);
	void SetBlendState(const FString& _Value);
	void SetDepthState(const FString& _Value);

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



	class std::shared_ptr<class FVertexShader> VertexShaderPtr = nullptr;
	class std::shared_ptr<class FPixelShader> PixelShaderPtr = nullptr;
	class std::shared_ptr<class FRasterizer> RasterizerPtr = nullptr;
	class std::shared_ptr<class FBlendState> BlendStatePtr = nullptr;
	class std::shared_ptr<class FDepthStencilState> DepthStencilPtr = nullptr;

};



