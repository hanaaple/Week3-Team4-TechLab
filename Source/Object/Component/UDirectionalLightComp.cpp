#include "UDirectionalLightComp.h"

#include "Resource/DirectResource/ShaderResourceBinding.h"

UDirectionalLightComp::UDirectionalLightComp()
{
	LightConstantData.lightDiffuseColor = FVector(1, 1, 1);
	LightConstantData.lightSpecularColor = FVector(1, 1, 1);


	FString BufferName = TEXT("DirectionalLight");
	uint32 DataSize = sizeof(FLightConstantData);

	std::shared_ptr<FConstantBuffer> Res = FConstantBuffer::Find(BufferName);

	if (Res == nullptr)
	{
		//없으면 그 사이즈의 이름으로 만듦 이름이 겹치면 몰?루
		Res = FConstantBuffer::Create("DirectionalLight", DataSize);
	}
	
	ConstantBufferBindingPtr = std::make_shared<FConstantBufferBinding>();

	ConstantBufferBindingPtr->DataSize = DataSize;
	ConstantBufferBindingPtr->CPUDataPtr = &LightConstantData;
	ConstantBufferBindingPtr->Res = Res;
	ConstantBufferBindingPtr->Name = BufferName;
	ConstantBufferBindingPtr->bIsUseVertexShader = true;
	ConstantBufferBindingPtr->bIsUsePixelShader = true;
	ConstantBufferBindingPtr->BindPoint = 4;
}

void UDirectionalLightComp::BeginPlay()
{
	USceneComponent::BeginPlay();
}

void UDirectionalLightComp::Tick(float DeltaTime)
{
	USceneComponent::Tick(DeltaTime);

	LightConstantData.lightDirection = GetRelativeTransform().GetForward();
	
	ConstantBufferBindingPtr->Setting();
}
