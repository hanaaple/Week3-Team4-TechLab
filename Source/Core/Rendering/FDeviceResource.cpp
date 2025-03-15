#include "FDevice.h"
#include "Resource/DirectResource/Vertexbuffer.h"
#include "Resource/DirectResource/IndexBuffer.h"
#include "Resource/DirectResource/PixelShader.h"
#include "Resource/DirectResource/VertexShader.h"
#include "Resource/DirectResource/InputLayout.h"

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
	
	FPixelShader::Load(L"Shaders/Font_PS.hlsl","Font_PS","Font_PS");

	
	
}
