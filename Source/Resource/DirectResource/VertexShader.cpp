#include "VertexShader.h"

#include <d3dcompiler.h>


void FVertexShader::ShaderLoad(const LPCWSTR& _Path, const FString& _EntryPoint, UINT _VersionHight, UINT _VersionLow)
{	// std::string*
	ID3DBlob* Error = nullptr;
	
	D3DCompileFromFile(_Path, nullptr, nullptr, *_EntryPoint, "vs_5_0", 0, 0, &BinaryCode, &Error);
	//Device->CreateVertexShader(VertexShaderCSO->GetBufferPointer(), VertexShaderCSO->GetBufferSize(), nullptr, &SimpleVertexShader);

}
