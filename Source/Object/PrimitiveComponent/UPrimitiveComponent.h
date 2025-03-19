#pragma once

#include "Core/Engine.h"
#include "Object/USceneComponent.h"
#include "Primitive/PrimitiveVertices.h"
#include "Resource/DirectResource/Vertexbuffer.h"
#include "Resource/DirectResource/IndexBuffer.h"
#include "Resource/RenderResourceCollection.h"


//상수버퍼로 객체의 정보를 넣을 구조체
struct alignas(16) FConstantsComponentData
{
	FMatrix MVP;
	FVector4 Color;
	// true인 경우 Vertex Color를 사용하고, false인 경우 Color를 사용합니다.
	FVector4 UUIDColor;
	uint32 bUseVertexColor;
	FVector Padding;
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
	virtual void RegisterComponentWithWorld(class UWorld* World);

public:
	void SetCanBeRendered(bool bRender) { bCanBeRendered = bRender; }

	void SetIsOrthoGraphic(bool IsOrtho) { bIsOrthoGraphic = IsOrtho; }
	bool GetIsOrthoGraphic() { return bIsOrthoGraphic;}

	void SetIsPicking(bool IsPicking) { bIsPicking = IsPicking; }
	bool GetIsPicking() { return bIsPicking; }
	FConstantsComponentData& GetConstantsComponentData() { return ConstantsComponentData; }
	//void SetConstantsComponentData(FConstantsComponentData& ) { bIsBillboard = bBillboard; }


	void SetMesh(const FString& _Name) { RenderResourceCollection.SetMesh(_Name); }
	void SetMaterial(const FString& _Name) { RenderResourceCollection.SetMaterial(_Name); }
	
	FRenderResourceCollection& GetRenderResourceCollection() { return RenderResourceCollection; }
	
protected:
	bool bCanBeRendered = false;
	bool bIsBillboard = false;
	bool bUseVertexColor = true;
	bool bIsOrthoGraphic = false;
	bool bIsPicking = false;

	FVector4 CustomColor = FVector4(1.0f, 1.0f, 1.0f, 1.0f);



	
private:
	FRenderResourceCollection RenderResourceCollection;
	FConstantsComponentData ConstantsComponentData;
};

class UCubeComp : public UPrimitiveComponent
{
	DECLARE_CLASS(UCubeComp, UPrimitiveComponent)

public:
	UCubeComp();

	virtual EPrimitiveType GetType() override
	{

		return EPrimitiveType::EPT_Cube;
	}
};

class USphereComp : public UPrimitiveComponent
{
	DECLARE_CLASS(USphereComp, UPrimitiveComponent)

public:
	USphereComp();

	virtual EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Sphere;
	}
};

class UTriangleComp : public UPrimitiveComponent
{
	DECLARE_CLASS(UTriangleComp, UPrimitiveComponent)

public:
	UTriangleComp();

	virtual EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Triangle;
	}
};

class ULineComp : public UPrimitiveComponent
{
	DECLARE_CLASS(ULineComp, UPrimitiveComponent)


public:
	ULineComp();

	virtual EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Line;
	}
};

class UCylinderComp : public UPrimitiveComponent
{
	DECLARE_CLASS(UCylinderComp, UPrimitiveComponent)


public:
	UCylinderComp();

	virtual EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Cylinder;
	}
};

class UConeComp : public UPrimitiveComponent
{
	DECLARE_CLASS(UConeComp, UPrimitiveComponent)

public:
	UConeComp();

	virtual EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Cone;
	}
};

class UQuadComp : public UPrimitiveComponent
{
	DECLARE_CLASS(UQuadComp, UPrimitiveComponent)

public:
	UQuadComp();

	virtual EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Quad;
	}
};