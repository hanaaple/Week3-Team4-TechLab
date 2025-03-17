#include "FDevice.h"
#include "Resource/DirectResource/Vertexbuffer.h"
#include "Resource/DirectResource/IndexBuffer.h"
#include "Resource/DirectResource/PixelShader.h"
#include "Resource/DirectResource/VertexShader.h"
#include "Resource/DirectResource/InputLayout.h"
#include "Resource/DirectResource/BlendState.h"
#include "Resource/DirectResource/Rasterizer.h"
#include "Resource/DirectResource/DepthStencilState.h"
#include "Resource/DirectResource/ConstantBuffer.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"
	

void FDevice::InitResource()
{
	std::shared_ptr<FVertexShader> VS = FVertexShader::Load(L"Shaders/ShaderW0.hlsl","Simple_VS","mainVS");
	FInputLayout::Create("Simple_VS" , VS);
	FPixelShader::Load(L"Shaders/ShaderW0.hlsl","Simple_PS","mainPS");

	FPixelShader::Load(L"Shaders/ShaderW0.hlsl","Picking_PS","PickingPS");
	{
		
		std::shared_ptr<FVertexShader> VS = FVertexShader::Load(L"Shaders/Font_VS.hlsl","Font_VS","Font_VS");
		//FInputLayout::Create("Font_VS" , VS);
	}

	FConstantBuffer::Create("DefaultConstantBuffer", sizeof(FConstantsComponentData));

	
	//FPixelShader::Load(L"Shaders/Font_PS.hlsl","Font_PS","Font_PS");


	
	
	D3D11_RASTERIZER_DESC RasterizerDesc = {};
	RasterizerDesc.FillMode = D3D11_FILL_SOLID; // 채우기 모드
	RasterizerDesc.CullMode = D3D11_CULL_BACK;  // 백 페이스 컬링
	RasterizerDesc.FrontCounterClockwise = FALSE;
	
	FRasterizer::Create("DefaultRasterizer", RasterizerDesc);

	{
		D3D11_DEPTH_STENCIL_DESC DepthStencilDesc = {};
		DepthStencilDesc.DepthEnable = true;
		// 깊이 테스트만 하고 안쓸수도 있다.
		// Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		DepthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		DepthStencilDesc.StencilEnable = false;

		FDepthStencilState::Create("DefaultDepthStencilState", DepthStencilDesc);
	}
	

	{
		// Blend
		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;

		// src srcColor * src의 알파
		// 1, 0, 0(, 1) * 1.0f
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		FBlendState::Create("DefaultBlendState", blendDesc);
	}
	
	{
		// Blend
		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;

		// src srcColor * src의 알파
		// 1, 0, 0(, 1) * 1.0f
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		//안개처럼 알파값이 계속 누적됨
		FBlendState::Create("AddBlendState", blendDesc);
	}
}
