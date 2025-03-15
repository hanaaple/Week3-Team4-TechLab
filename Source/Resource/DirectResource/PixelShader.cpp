#include "PixelShader.h"

#include <d3dcompiler.h>
#include "Core/Rendering/FDevice.h"
#include "Debug/DebugConsole.h"

void FPixelShader::Setting()
{
	if (nullptr == ShaderPtr)
	{
		UE_LOG("Error: FPixelShader Setting Failed");
	}
	
	FDevice::Get().GetDeviceContext()->PSSetShader(ShaderPtr, nullptr, 0);
}

void FPixelShader::ShaderLoad(const LPCWSTR& _Path, const FString& _EntryPoint, UINT _VersionHight, UINT _VersionLow)
{
	ID3DBlob* Error = nullptr;
	
	D3DCompileFromFile(_Path, nullptr, nullptr, *_EntryPoint, "ps_5_0", 0, 0, &BinaryCode, &Error);
	FDevice::Get().GetDevice()->CreatePixelShader(BinaryCode->GetBufferPointer(), BinaryCode->GetBufferSize(), nullptr, &ShaderPtr);
}
