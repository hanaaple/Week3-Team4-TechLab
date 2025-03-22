#pragma once
#include "MeshComponent.h"

class UStaticMeshComponent : public UMeshComponent
{
	using Super = UMeshComponent;
	DECLARE_CLASS(UStaticMeshComponent, Super)

public:
	UStaticMeshComponent()
	{
	}
	//virtual ~UStaticMeshComponent() override;
public:
	// virtual void BeginPlay() override;
	// virtual void Tick(float DeltaTime) override;
	// virtual void EndPlay(EEndPlayReason::Type Reason) override;
	// virtual void Render() override;


	
public:
	
	//UStaticMesh* StaticMesh;

	//void NotifyIfStaticMeshChanged();

	//virtual EPrimitiveType GetType() override;
	
	void SetStaticMeshInternal(UStaticMesh* NewStaticMesh) { RenderResourceCollection.SetMesh(NewStaticMesh->GetFName().ToString()); }
	//virtual bool SetStaticMesh(class UStaticMesh* NewMesh);
	/** Get the StaticMesh used by this instance. */

	void SetStaticMesh(const FString& InName) { RenderResourceCollection.SetMesh(InName); }
	std::shared_ptr<UStaticMesh> GetStaticMesh() const { return RenderResourceCollection.GetMesh(); }

	//void GetLocalBounds(FVector& Min, FVector& Max) const;

	// ENGINE_API virtual void Serialize(FArchive& Ar) override;
	// void Serialize(bool bIsLoading, Json Handle)
	// {
	// 	Super::Serialize(IsLoading, Handle);
	//
	// 	if(bIsLoading)
	// 	{
	// 		FString assetName;
	// 		Handle << "ObjStaticMeshAsset" << assetName;
	// 		StaticMesh = FObjManager::LoadObjStaticMesh(assetName);
	// 	}
	// 	else
	// 	{
	// 		FString assetName = StaticMesh->GetAssetPathFileName();
	// 		Handle << "ObjStaticMeshAsset" << assetName;
	// 	}
	// }
};

class UCubeComp : public UStaticMeshComponent
{
	DECLARE_CLASS(UCubeComp, UStaticMeshComponent)

public:
	UCubeComp();

	virtual EPrimitiveType GetType() override
	{

		return EPrimitiveType::EPT_Cube;
	}
};

class USphereComp : public UStaticMeshComponent
{
	DECLARE_CLASS(USphereComp, UStaticMeshComponent)

public:
	USphereComp();

	virtual EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Sphere;
	}

	/**
	* Change the sphere radius. This is the unscaled radius, before component scale is applied.
	* @param	InSphereRadius: the new sphere radius
	*/
	void SetSphereRadius(float InSphereRadius);

	// @return the radius of the sphere, with component scale applied.
	inline float GetScaledSphereRadius() const { return Radius * GetShapeScale(); }

	// @return the radius of the sphere, ignoring component scale.
	inline float GetUnscaledSphereRadius() const { return Radius; }

	//~ Begin UPrimitiveComponent Interface.
	virtual inline bool IsZeroExtent() { return Radius == 0.0f; }
	//virtual struct FCollisionShape GetCollisionShape(float Inflation = 0.0f) const override;
	//~ End UPrimitiveComponent Interface.

	//~ Begin USceneComponent Interface
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	//~ End USceneComponent Interface

	// Get the scale used by this shape. This is a uniform scale that is the minimum of any non-uniform scaling.
	// @return the scale used by this shape.
	float GetShapeScale() const;

	// Sets the sphere radius without triggering a render or physics update.
	inline void InitSphereRadius(float InSphereRadius) { Radius = InSphereRadius; }

protected:
	float Radius;
};

class UTriangleComp : public UStaticMeshComponent
{
	DECLARE_CLASS(UTriangleComp, UStaticMeshComponent)

public:
	UTriangleComp();

	virtual EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Triangle;
	}
};

class ULineComp : public UStaticMeshComponent
{
	DECLARE_CLASS(ULineComp, UStaticMeshComponent)


public:
	ULineComp();

	virtual EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Line;
	}
};

class UCylinderComp : public UStaticMeshComponent
{
	DECLARE_CLASS(UCylinderComp, UStaticMeshComponent)


public:
	UCylinderComp();

	virtual EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Cylinder;
	}
};

class UConeComp : public UStaticMeshComponent
{
	DECLARE_CLASS(UConeComp, UStaticMeshComponent)

public:
	UConeComp();

	virtual EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Cone;
	}
};

class UQuadComp : public UStaticMeshComponent
{
	DECLARE_CLASS(UQuadComp, UStaticMeshComponent)

public:
	UQuadComp();

	virtual EPrimitiveType GetType() override
	{
		return EPrimitiveType::EPT_Quad;
	}
};