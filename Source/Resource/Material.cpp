#include "Material.h"

#include "Texture.h"
#include "Core/Rendering/FDevice.h"
#include "Debug/DebugConsole.h"
#include "Resource/DirectResource/BlendState.h"
#include "Resource/DirectResource/DepthStencilState.h"
#include "Resource/DirectResource/VertexShader.h"
#include "Resource/DirectResource/PixelShader.h"
#include "Resource/DirectResource/Rasterizer.h"

FMaterial::FMaterial()
{
	SetRasterizer("DefaultRasterizer");
	SetBlendState("DefaultBlendState");
	SetDepthState("DefaultDepthStencilState");
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

void FMaterial::Texture()
{
	for (const auto& [Slot, TexturePtr] : TexturePtrMap)
	{
		if (TexturePtr != nullptr)
		{
			TexturePtr->VSSetting(Slot);
			TexturePtr->PSSetting(Slot);
		}
		else
		{
			ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
			FDevice::Get().GetDeviceContext()->PSSetShaderResources(Slot, 1, nullSRV);  // 언바인딩
			FDevice::Get().GetDeviceContext()->VSSetShaderResources(Slot, 1, nullSRV);  // 언바인딩
		}
	}
}

void FMaterial::UpdateConstantBuffer()
{
	if (ConstantBufferBindingPtr != nullptr)
		ConstantBufferBindingPtr->Setting();
}

void FMaterial::SetVertexShader(const FString& InValue)
{
	VertexShaderPtr = FVertexShader::Find(InValue);

	if (nullptr == VertexShaderPtr)
	{
		MsgBoxAssert("존재하지 않는 버텍스 쉐이더를 세팅하려고 했습니다.");
	}
}

void FMaterial::SetRasterizer(const FString& InValue)
{
	RasterizerPtr = FRasterizer::Find(InValue);

	if (nullptr == RasterizerPtr)
	{
		MsgBoxAssert("존재하지 않는 레스터 라이저를 세팅하려고 했습니다.");
		return;
	}
}

void FMaterial::SetPixelShader(const FString& InValue)
{
	PixelShaderPtr = FPixelShader::Find(InValue);

	if (nullptr == PixelShaderPtr)
	{
		MsgBoxAssert("존재하지 않는 픽셀쉐이더를 세팅하려고 했습니다.");
		return;
	}
}

void FMaterial::SetBlendState(const FString& InValue)
{
	BlendStatePtr = FBlendState::Find(InValue);

	if (nullptr == BlendStatePtr)
	{
		MsgBoxAssert("존재하지 않는 블랜드를 세팅하려고 했습니다.");
		return;
	}
}

void FMaterial::SetDepthState(const FString& InValue)
{
	DepthStencilPtr = FDepthStencilState::Find(InValue);

	if (nullptr == DepthStencilPtr)
	{
		MsgBoxAssert("존재하지 않는 깊이버퍼 세팅을 사용하려고 했습니다.");
		return;
	}
}

void FMaterial::SetTexture(const FString& InValue, uint8 InSlot)
{
	auto TexturePtr = FTexture::Find(InValue);
	TexturePtrMap.Add(InSlot, TexturePtr);
}

void FMaterial::SetConstantBuffer(const FString& InValue, const void* CPUDataPtr, int DataSize, int BindPoint, bool bIsUseVertexShader,	bool bIsUsePixelShader)
{
	std::shared_ptr<FConstantBuffer> Res = FConstantBuffer::Find(InValue);

	if (Res == nullptr)
	{
		//없으면 그 사이즈의 이름으로 만듦 이름이 겹치면 몰?루
		Res = FConstantBuffer::Create(InValue, DataSize);
	}
	
	ConstantBufferBindingPtr = std::make_shared<FConstantBufferBinding>();

	ConstantBufferBindingPtr->DataSize = DataSize;
	ConstantBufferBindingPtr->CPUDataPtr = CPUDataPtr;
	ConstantBufferBindingPtr->Res = Res;
	ConstantBufferBindingPtr->Name = InValue;
	ConstantBufferBindingPtr->bIsUseVertexShader = bIsUseVertexShader;
	ConstantBufferBindingPtr->bIsUsePixelShader = bIsUsePixelShader;
	ConstantBufferBindingPtr->BindPoint = BindPoint;
}

void FMaterial::Setting()
{
	VertexShader();
	Rasterizer();
	PixelShader();
	Blend();
	DepthStencil();
	Texture();
	UpdateConstantBuffer();
}
