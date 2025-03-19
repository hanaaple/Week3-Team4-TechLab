#pragma once
#include "Object/USceneComponent.h"

// 모든 조명 컴포넌트의 기본 클래스
class ULightComponentBase : public USceneComponent
{
	DECLARE_CLASS(ULightComponentBase, USceneComponent)

public:
	ULightComponentBase();
	virtual ~ULightComponentBase() override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Render();

	// 조명 활성화/비활성화
	void SetLightEnabled(bool bEnabled);
	bool IsLightEnabled() const { return bLightEnabled; }

	// 조명 색상 설정
	void SetLightColor(const FVector4& InColor);
	const FVector4& GetLightColor() const { return LightColor; }

	// 조명 강도 설정
	void SetIntensity(float InIntensity);
	float GetIntensity() const { return Intensity; }

	// 그림자 캐스팅 설정
	void SetCastShadows(bool bCast);
	bool GetCastShadows() const { return bCastShadows; }

protected:
	// 조명 활성화 여부
	bool bLightEnabled;

	// 조명 색상 (RGB + 강도)
	FVector4 LightColor;

	// 조명 강도
	float Intensity;

	// 그림자 캐스팅 여부
	bool bCastShadows;

	// 조명 범위
	float AttenuationRadius;
};
