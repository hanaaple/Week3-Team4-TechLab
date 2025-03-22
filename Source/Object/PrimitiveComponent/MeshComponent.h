#pragma once
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"

class UMeshComponent : public UPrimitiveComponent
{
	using Super = UPrimitiveComponent;
	DECLARE_CLASS(UMeshComponent, Super)
	
public:
	UMeshComponent(){}
	// virtual ~UMeshComponent() override;
	
public:
	// virtual void BeginPlay() override;
	// virtual void Tick(float DeltaTime) override;
	// virtual void EndPlay(EEndPlayReason::Type Reason) override;
	// virtual void Render() override;

	


public:
	
	//virtual EPrimitiveType GetType() override;
	// ENGINE_API virtual TArray<class UMaterialInterface*> GetMaterials() const;


	//~ Begin UPrimitiveComponent Interface
	// ENGINE_API virtual int32 GetNumMaterials() const override;
	// ENGINE_API virtual UMaterialInterface* GetMaterial(int32 ElementIndex) const override;
	// ENGINE_API virtual UMaterialInterface* GetMaterialByName(FName MaterialSlotName) const override;
	// ENGINE_API virtual void SetMaterial(int32 ElementIndex, UMaterialInterface* Material) override;
	// ENGINE_API virtual void SetMaterialByName(FName MaterialSlotName, class UMaterialInterface* Material) override;
	// ENGINE_API virtual void GetUsedMaterials(TArray<UMaterialInterface*>& OutMaterials, bool bGetDebugMaterials = false) const override;
	//~ End UPrimitiveComponent Interface
};
