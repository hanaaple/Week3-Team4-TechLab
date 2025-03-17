#include "ShaderResourceBinding.h"
#include "Debug/DebugConsole.h"

void FConstantBufferBinding::Setting()
{
	ShaderType Type = ParentShader->GetShaderType();

	switch (Type)
	{
	case ShaderType::Vertex:
		Res->VSSetting(BindPoint);
		break;
	case ShaderType::Pixel:
		Res->PSSetting(BindPoint);
		break;
	// case ShaderType::Compute:
	// 	Res->CSSetting(BindPoint);
	// 	break;
	default:
		MsgBoxAssert("처리할수 없는 쉐이더 세팅 유형입니다.");
		break;
	}
}

void FConstantBufferBinding::Reset()
{
}
