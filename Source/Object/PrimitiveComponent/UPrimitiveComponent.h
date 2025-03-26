#pragma once

#define _TCHAR_DEFINED
#include <d3dcommon.h>

#include "Core/Engine.h"
#include "Core/Math/Matrix.h"
#include "Object/USceneComponent.h"
#include "Primitive/PrimitiveVertices.h"
#include "Resource/RenderResourceCollection.h"
#include "Resource/StaticMesh.h"


class FVertexShader;
class FRasterizer;
class FDepthStencilState;
class FBlendState;
class FPixelShader;
class FConstantBufferBinding;
class FConstantBuffer;
class FInputLayout;


//상수버퍼로 객체의 정보를 넣을 구조체
struct alignas(16) FConstantsComponentData
{
	FMatrix MVP;
	FVector4 Color;
	// true인 경우 Vertex Color를 사용하고, false인 경우 Color를 사용합니다.
	FVector4 UUIDColor;
	uint32 bUseVertexColor;
	FMatrix M;
};

class UPrimitiveComponent : public USceneComponent
{
	DECLARE_CLASS(UPrimitiveComponent, USceneComponent)

public:
	UPrimitiveComponent();
	virtual ~UPrimitiveComponent() override;

public:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//void UpdateConstantPicking(const URenderer& Renderer, FVector4 UUIDColor) const;
	//void UpdateConstantDepth(const URenderer& Renderer, int Depth) const;
	virtual void Render();
	void RenderGizmo();
	virtual void CalculateModelMatrix(FMatrix& OutMatrix);

	virtual EPrimitiveType GetType() { return EPrimitiveType::EPT_None; }

	bool IsUseVertexColor() const { return bUseVertexColor; }

	void SetCustomColor(const FVector4& InColor)
	{
		CustomColor = InColor; 
		bUseVertexColor = false;
	}

	void SetUseVertexColor(bool bUse)
	{
		bUseVertexColor = bUse;
	}
	const FVector4& GetCustomColor() const { return CustomColor; }

public:
	virtual void RegisterComponentWithWorld(UWorld* World);

public:
	void SetCanBeRendered(bool bRender) { bCanBeRendered = bRender; }

	void SetIsOrthoGraphic(bool IsOrtho) { bIsOrthoGraphic = IsOrtho; }
	bool GetIsOrthoGraphic() const { return bIsOrthoGraphic;}

	void SetIsPicking(bool IsPicking) { bIsPicking = IsPicking; }
	bool GetIsPicking() const { return bIsPicking; }
	FConstantsComponentData& GetConstantsComponentData() { return ConstantsComponentData; }
	//void SetConstantsComponentData(FConstantsComponentData& ) { bIsBillboard = bBillboard; }

	std::shared_ptr<FMaterial> GetMaterial() const { return RenderResourceCollection.GetMaterial(); }

	void SetMaterial(const FString& InName) { RenderResourceCollection.SetMaterial(InName); }
	
	FRenderResourceCollection& GetRenderResourceCollection() { return RenderResourceCollection; }
protected:
	bool bCanBeRendered = false;
	bool bIsBillboard = false;
	bool bUseVertexColor = true;
	bool bIsOrthoGraphic = false;
	bool bIsPicking = false;

	FVector4 CustomColor = FVector4(1.0f, 1.0f, 1.0f, 1.0f);

protected:
	FRenderResourceCollection RenderResourceCollection;
	FConstantsComponentData ConstantsComponentData;

	// Bounding Box
public:
	void SetBoundsScale(float NewBoundScale);
	void SetBoxExtent(const FVector& InExtent);

	inline FVector GetScaledBoxExtent() const { return BoxExtent * GetComponentTransform().GetScale(); }

	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;

	inline void InitBoxExtent(const FVector& InExtent) { BoxExtent = InExtent; }

private:
	float BoundsScale = 1.0f;
	FVector BoxExtent;
};