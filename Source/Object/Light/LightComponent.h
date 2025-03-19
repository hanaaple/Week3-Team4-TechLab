#pragma once
#include "LightComponentBase.h"

// 일반 조명 컴포넌트 (점광, 방향광 등의 공통 기능)
class ULightComponent : public ULightComponentBase
{
	DECLARE_CLASS(ULightComponent, ULightComponentBase)

public:
	ULightComponent();
	virtual ~ULightComponent() override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Render();

	// 감쇠 반경 설정
	virtual void SetAttenuationRadius(float InRadius);
	float GetAttenuationRadius() const { return AttenuationRadius; }

	// 감쇠 지수 설정
	void SetAttenuationExponent(float InExponent);
	float GetAttenuationExponent() const { return AttenuationExponent; }

	// 조명 영향 범위 시각화 메시 업데이트
	virtual void UpdateLightVisualization();

protected:
	// 감쇠 지수 (빛이 거리에 따라 약해지는 정도)
	float AttenuationExponent;

	// 조명 영향 범위 시각화 메시 생성
	virtual void CreateLightVisualizationMesh();
};
