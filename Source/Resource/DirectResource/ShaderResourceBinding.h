#pragma once
#include "Core/Container/String.h"
#include "ConstantBuffer.h"
#include "Shader.h"

//리소스와 다르게 각 객체가 가지게 됨
class FShaderResourceBinding
{
public:
	FString Name; // Transformdata <= 쉐이더에서 사용하는 리소스의 이름
	FShader* ParentShader; // <= 날 사용하는 쉐이더
	int BindPoint = -1; // <= b0 t0 s0 ... s1 s2 s3, b1 b2 b3 몇번 상수버퍼냐

public:
	virtual void Setting();
	virtual void Reset();
	
};

//쉐이더가 포인터를 받아 상수버퍼를 바인딩하기 위한 클래스
class FConstantBufferBinding  : public FShaderResourceBinding
{
public:
	std::shared_ptr<class FConstantBuffer> Res;

	const void* CPUDataPtr = nullptr;
	int DataSize = -1;

	void Setting() override;
	void Reset() override;
};