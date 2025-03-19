#pragma once
#include "LightComponent.h"
#include "Resource/RenderResourceCollection.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"


// 스포트라이트 컴포넌트
class USpotLightComponent : public ULightComponent
{
	DECLARE_CLASS(USpotLightComponent, ULightComponent)

public:
	USpotLightComponent();
	virtual ~USpotLightComponent() override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void Render();

	void SetAttenuationRadius(float InRadius);

	// 내부 원뿔 각도 설정 (라디안)
	void SetInnerConeAngle(float InAngle);
	float GetInnerConeAngle() const { return InnerConeAngle; }

	// 외부 원뿔 각도 설정 (라디안)
	void SetOuterConeAngle(float InAngle);
	float GetOuterConeAngle() const { return OuterConeAngle; }

	// 원뿔 시각화 메시 업데이트
	virtual void UpdateLightVisualization() override;

	// 경계 계산
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;

	UPrimitiveComponent* GuideMesh;
	FRenderResourceCollection& GetRenderResourceCollection() { return RenderResourceCollection; }
	FConstantsComponentData& GetConstantsComponentData() { return ConstantsComponentData; }
protected:
	TArray<FVertexSimple> VertexBuffer;
	TArray<uint32> IndexBuffer;
	FRenderResourceCollection RenderResourceCollection;

	// 내부 원뿔 각도 (라디안)
	float InnerConeAngle;

	// 외부 원뿔 각도 (라디안)
	float OuterConeAngle;

	// 원뿔 높이 (조명 거리)
	float ConeHeight;

	// 조명 원뿔 시각화 메시 생성
	virtual void CreateLightVisualizationMesh() override;

	// 상수 버퍼에 전달할 스포트라이트 데이터
	struct FSpotLightData
	{
		FVector4 Position;
		FVector4 Direction;
		FVector4 Color;
		float InnerConeAngle;
		float OuterConeAngle;
		float AttenuationRadius;
		float Intensity;
		int CastShadows;
		FVector Padding;
	};


	FConstantsComponentData ConstantsComponentData;
	// 스포트라이트 데이터 상수 버퍼
	FSpotLightData SpotLightData;
};
