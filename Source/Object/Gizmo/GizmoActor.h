#pragma once
#include "object/actor/actor.h"

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


class AGizmoActor : public AActor
{
public:
	AGizmoActor();
	virtual ~AGizmoActor() = default;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetSelectedAxis(ESelectedAxis InAxis) { SelectedAxis = InAxis; }
	ESelectedAxis GetSelectedAxis() const { return SelectedAxis; }

	EGizmoType GetGizmoType() const { return GizmoType; }

private:
	ESelectedAxis SelectedAxis = ESelectedAxis::None;
	EGizmoType GizmoType = EGizmoType::Translate;
};

