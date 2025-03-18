#include "ShaderResourceBinding.h"
#include "Debug/DebugConsole.h"

void FConstantBufferBinding::Setting()
{

	if (nullptr == CPUDataPtr)
	{
		MsgBoxAssert("상수버퍼를 세팅해주지 않았습니다.")
	}

	//ShaderType Type = ParentShader->GetShaderType();

	Res->ChangeData(CPUDataPtr, DataSize);
	
	if (bIsUseVertexShader == true)
	{
		Res->VSSetting(BindPoint);
	}
	if (bIsUsePixelShader == true)
	{
		Res->PSSetting(BindPoint);
	}
	
	// switch (bIsUsePixelShader )
	// {
	// case ShaderType::Vertex:
	// 	Res->VSSetting(BindPoint);
	// 	break;
	// case ShaderType::Pixel:
	// 	Res->PSSetting(BindPoint);
	// 	break;
	// // case ShaderType::Compute:
	// // 	Res->CSSetting(BindPoint);
	// // 	break;
	// default:
	// 	MsgBoxAssert("처리할수 없는 쉐이더 세팅 유형입니다.")
	// 	break;
	// }
}

void FConstantBufferBinding::Reset()
{
}
