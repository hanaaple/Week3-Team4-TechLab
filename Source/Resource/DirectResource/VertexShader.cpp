#include "VertexShader.h"

#include <d3dcompiler.h>
#include "Core/Rendering/FDevice.h"

#include "Debug/DebugConsole.h"


FVertexShader::FVertexShader()
{
}

FVertexShader::~FVertexShader()
{
		if (nullptr != ShaderPtr)
	{
		ShaderPtr->Release();
		ShaderPtr = nullptr;
	}
}

void FVertexShader::Setting()
{

	if (nullptr == ShaderPtr)
	{
		MsgBoxAssert("Error: FVertexShader Setting Failed");
	}
	
	// 버텍스버퍼를 여러개 넣어줄수 있다.
	FDevice::Get().GetDeviceContext()->VSSetShader(ShaderPtr, nullptr, 0);
}

void FVertexShader::ShaderLoad(const LPCWSTR& _Path, const FString& _EntryPoint, UINT _VersionHight, UINT _VersionLow)
{	// std::string*
	ID3DBlob* Error = nullptr;
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;

	SetShaderType(ShaderType::Vertex);
	
	D3DCompileFromFile(_Path, nullptr, nullptr, _EntryPoint.c_char(), "vs_5_0", shaderFlags, 0, &BinaryCode, &Error);

	if (Error)
	{
		const char* a = (char*)Error->GetBufferPointer();
		OutputDebugStringA((char*)Error->GetBufferPointer()); // 오류 메시지 출력
		Error->Release();
	}
	FDevice::Get().GetDevice()->CreateVertexShader(BinaryCode->GetBufferPointer(), BinaryCode->GetBufferSize(), nullptr, &ShaderPtr);
}
