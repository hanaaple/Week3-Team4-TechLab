#pragma once

#define _TCHAR_DEFINED
#include <d3d11.h>

#include "Resource/Resource.h"
#include "Core/Container/String.h"
#include "Core/Container/Array.h"
#include "Shader.h"
#include "Core/hal/PlatformType.h"



class FVertexShader :
	public FResource<FVertexShader> , FShader
{
	friend class FInputLayout;
	
public:
	static std::shared_ptr<FVertexShader> Load(const LPCWSTR& _Path, const FString& _Name, const FString&  _EntryPoint, UINT _VersionHight = 5, UINT _VersionLow = 0)
	{
		std::shared_ptr<FVertexShader> Res = FVertexShader::CreateRes(_Name);
		Res->ShaderLoad(_Path, _EntryPoint, _VersionHight, _VersionLow);
		return Res;
	}

	
	void Setting();
	
private:
	ID3D11VertexShader* ShaderPtr = nullptr;
	
	void ShaderLoad(const LPCWSTR&  _Path, const FString&  _EntryPoint, UINT _VersionHight = 5, UINT _VersionLow = 0);
};
