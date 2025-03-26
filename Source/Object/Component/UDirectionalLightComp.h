#pragma once
#include "Object/USceneComponent.h"

class FConstantBufferBinding;

struct alignas(16) FLightConstantData
{
	FVector lightDirection;
	FVector lightDiffuseColor;
	FVector lightSpecularColor;
};

class UDirectionalLightComp : public USceneComponent
{
	DECLARE_CLASS(UDirectionalLightComp, USceneComponent)

public:
	UDirectionalLightComp();
	virtual ~UDirectionalLightComp() = default;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	// Mat->AddConstantBuffer(MaterialName + TEXT("Light"), &StaticMesh->GetCameraConstantData(), sizeof(FCameraConstantData), 4, true, true);
	// lightDirection = normalize(float3(1, 1, -1));
	FLightConstantData LightConstantData;
	std::shared_ptr<FConstantBufferBinding> ConstantBufferBindingPtr;
};
