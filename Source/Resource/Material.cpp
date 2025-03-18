#include "Material.h"

#include "Resource/DirectResource/BlendState.h"
#include "Resource/DirectResource/DepthStencilState.h"
#include "Resource/DirectResource/VertexShader.h"
#include "Resource/DirectResource/PixelShader.h"
#include "Resource/DirectResource/Rasterizer.h"

FMaterial::FMaterial()
{
	
}

FMaterial::~FMaterial()
{
}

void FMaterial::VertexShader()
{
	if (nullptr == VertexShaderPtr)
	{
		MsgBoxAssert("존재하지 않는 버텍스 쉐이더를 세팅하려고 했습니다.");
	}

	VertexShaderPtr->Setting();
}

void FMaterial::Rasterizer()
{
	if (nullptr == RasterizerPtr)
	{
		MsgBoxAssert("존재하지 않는 레스터 라이저를 세팅하려고 했습니다.");
	}

	RasterizerPtr->Setting();
}

void FMaterial::PixelShader()
{
	if (nullptr == PixelShaderPtr)
	{
		MsgBoxAssert("존재하지 않는 레스터 라이저를 세팅하려고 했습니다.");
	}

	PixelShaderPtr->Setting();
}

void FMaterial::Blend()
{
	if (nullptr == BlendStatePtr)
	{
		MsgBoxAssert("존재하지 않는 레스터 라이저를 세팅하려고 했습니다.");
	}

	BlendStatePtr->Setting();
}

void FMaterial::DepthStencil()
{
	// if (false == IsDepth)
	// {
	// 	GetContext()->OMSetDepthStencilState(nullptr, 0);
	// 	return;
	// }

	if (nullptr == DepthStencilPtr)
	{
		MsgBoxAssert("존재하지 않는 깊이 체크 세팅을 세팅하려고 했습니다.");
	}

	DepthStencilPtr->Setting();
}

void FMaterial::SetVertexShader(const FString& _Value)
{
	VertexShaderPtr = FVertexShader::Find(_Value);

	if (nullptr == VertexShaderPtr)
	{
		MsgBoxAssert("존재하지 않는 버텍스 쉐이더를 세팅하려고 했습니다.");
	}
}

void FMaterial::SetRasterizer(const FString& _Value)
{
	RasterizerPtr = FRasterizer::Find(_Value);

	if (nullptr == RasterizerPtr)
	{
		MsgBoxAssert("존재하지 않는 레스터 라이저를 세팅하려고 했습니다.");
		return;
	}
}

void FMaterial::SetPixelShader(const FString& _Value)
{
	PixelShaderPtr = FPixelShader::Find(_Value);

	if (nullptr == PixelShaderPtr)
	{
		MsgBoxAssert("존재하지 않는 픽셀쉐이더를 세팅하려고 했습니다.");
		return;
	}
}

void FMaterial::SetBlendState(const FString& _Value)
{
	BlendStatePtr = FBlendState::Find(_Value);

	if (nullptr == BlendStatePtr)
	{
		MsgBoxAssert("존재하지 않는 블랜드를 세팅하려고 했습니다.");
		return;
	}
}

void FMaterial::SetDepthState(const FString& _Value)
{
	DepthStencilPtr = FDepthStencilState::Find(_Value);

	if (nullptr == DepthStencilPtr)
	{
		MsgBoxAssert("존재하지 않는 깊이버퍼 세팅을 사용하려고 했습니다.");
		return;
	}
}

 void FMaterial::Setting()
{
	VertexShader();
	Rasterizer();
	PixelShader();
	Blend();
	DepthStencil();

}
