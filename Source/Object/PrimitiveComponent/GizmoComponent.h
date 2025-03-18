#pragma once
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"


enum class EAxis : uint8
{
	None,
	X,
	Y,
	Z
};

enum class EGizmoType : uint8
{
	Translate,
	Rotate,
	Scale,
	Max
};

class UGizmoComponent : public UPrimitiveComponent
{
	DECLARE_CLASS(UGizmoComponent, UPrimitiveComponent)

public:
	UGizmoComponent();

	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void EndPlay(EEndPlayReason::Type Reason) override;

	void Render() override;

	EPrimitiveType GetType() const { return EPrimitiveType::EPT_Gizmo; }

private:
	void OnChangedGizmoType(EGizmoType Gizmo);

private:
	
};

