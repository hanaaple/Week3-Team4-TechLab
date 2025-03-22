#pragma once
#include "StaticMeshComponent.h"

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

class UGizmoComponent : public UStaticMeshComponent
{
	DECLARE_CLASS(UGizmoComponent, UStaticMeshComponent)

	friend class AGizmoActor;

public:
	UGizmoComponent();

	// virtual void BeginPlay() override;
	// virtual void Tick(float DeltaTime) override;
	// virtual void EndPlay(EEndPlayReason::Type Reason) override;
	//
	// virtual void Render() override;

	EPrimitiveType GetType() override { return EPrimitiveType::EPT_Gizmo; }

	ESelectedAxis GetSelectedAxis() const { return Axis; }

private:
	void OnChangedGizmoType(EGizmoType Gizmo);

	ESelectedAxis Axis = ESelectedAxis::None;
};

