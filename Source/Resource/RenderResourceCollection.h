#pragma once

#include "Core/Engine.h"
#include "Resource/DirectResource/Vertexbuffer.h"
#include "Resource/DirectResource/IndexBuffer.h"

enum class RenderMode
{
	Indexed,
	Instancing,
};


class FRenderResourceCollection
{
public:


	//테스트 임시 메쉬



	void SetMesh(const FString& _Name);
	void SetMaterial(const FString& _Name);

	
	void SetMesh(std::shared_ptr<class FMesh> _Mesh);
	void SetMaterial(std::shared_ptr<class FMaterial> _Material);

	std::shared_ptr<FMesh> GetMesh()
	{
		return Mesh;
	}

	std::shared_ptr<FMaterial> GetMaterial()
	{
		return Material;
	}

	void Render();


	template<typename ConstantType>
	std::shared_ptr<class FConstantBufferBinding> SetConstantBufferBinding(const FString& _Name,  const ConstantType* DataPtr,
	 int _BindPoint, bool bIsUseVertexShader, bool bIsUsePixelShader)
	{
		return SetConstantBufferBinding(_Name, DataPtr,sizeof(ConstantType),_BindPoint,  bIsUseVertexShader, bIsUsePixelShader);
	}
	
	
	std::shared_ptr<class FConstantBufferBinding> SetConstantBufferBinding(const FString& _Name,
		const void* _CPUDataPtr, int _DataSize,int _BindPoint,
bool	bIsUseVertexShader, bool bIsUsePixelShader);

	
private:
	//class UPrimitiveComponent* ParentRenderer = nullptr;

	RenderMode Mode = RenderMode::Indexed;

	//원래는 메쉬와 메테리얼의 정보 둘다 필요함 일단 버텍스 쉐이더 정보만 받아서 자동화
	std::shared_ptr<class FInputLayout> Layout = nullptr;
	

	std::shared_ptr<class FMesh> Mesh = nullptr;
	std::shared_ptr<class FMaterial> Material = nullptr;

	
	TMap<FString, std::shared_ptr<class FConstantBufferBinding>> ConstantBufferBindings;
	
	// // 테스트 상수버퍼
	// std::shared_ptr<class FConstantBufferBinding> ConstantBufferBinding = nullptr;
	// std::shared_ptr<class FConstantBuffer> ConstantBuffer = nullptr;


};