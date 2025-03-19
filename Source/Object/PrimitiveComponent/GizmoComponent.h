#pragma once
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"

enum class ESelectedAxis : uint8
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

	friend class AGizmoActor;

public:
	UGizmoComponent();
	virtual ~UGizmoComponent() = default;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void EndPlay(EEndPlayReason::Type Reason) override;

	void Render() override;

	EPrimitiveType GetType() const { return EPrimitiveType::EPT_Gizmo; }

	ESelectedAxis GetSelectedAxis() const { return Axis; }

private:
	void OnChangedGizmoType(EGizmoType Gizmo);

	ESelectedAxis Axis = ESelectedAxis::None;

private:
	
};

