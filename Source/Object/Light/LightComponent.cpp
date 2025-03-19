#include "LightComponent.h"
#include "Core/Engine.h"
#include "Core/Rendering/URenderer.h"
#include "Object/World/World.h"
#include "Object/Actor/Camera.h"

ULightComponent::ULightComponent()
	: AttenuationExponent(1.0f)
{
	// 기본 감쇠 반경 설정 (부모 클래스에서 정의됨)
	AttenuationRadius = 1000.0f;

	// 기본 색상 설정 (흰색)
	SetLightColor(FVector4(1.0f, 1.0f, 1.0f, 1.0f));

	// 기본 강도 설정
	SetIntensity(1.0f);

	// 시각화 메시 생성
	CreateLightVisualizationMesh();
}

ULightComponent::~ULightComponent()
{
}

void ULightComponent::BeginPlay()
{
	ULightComponentBase::BeginPlay();
}

void ULightComponent::Tick(float DeltaTime)
{
	ULightComponentBase::Tick(DeltaTime);
}

void ULightComponent::Render()
{
	if (!bLightEnabled)
		return;

	ULightComponentBase::Render();

	// 조명 데이터를 셰이더에 전달하는 코드
	URenderer* Renderer = UEngine::Get().GetRenderer();
	if (Renderer)
	{
		// 조명 데이터를 상수 버퍼에 업데이트

		// 시각화 메시 렌더링
	}
}

void ULightComponent::SetAttenuationRadius(float InRadius)
{
	AttenuationRadius = InRadius;
	UpdateLightVisualization();
}

void ULightComponent::SetAttenuationExponent(float InExponent)
{
	AttenuationExponent = InExponent;
}

void ULightComponent::UpdateLightVisualization()
{
	// 시각화 메시 업데이트
	CreateLightVisualizationMesh();
}

void ULightComponent::CreateLightVisualizationMesh()
{
	// 기본 구현은 빈 상태로 두고, 자식 클래스에서 구체적인 메시 생성 로직 구현
	// 예: 점광원은 구체, 스포트라이트는 원뿔 등
}
