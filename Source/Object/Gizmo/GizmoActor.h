#pragma once
#include "object/actor/actor.h"
#include "Object/PrimitiveComponent/GizmoComponent.h"

class AGizmoActor : public AActor
{
public:
	AGizmoActor();
	virtual ~AGizmoActor() = default;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetSelectedAxis(EAxis InAxis) { SelectedAxis = InAxis; }
	EAxis GetSelectedAxis() const { return SelectedAxis; }

	EGizmoType GetGizmoType() const { return GizmoType; }

private:
	EAxis SelectedAxis = EAxis::None;
	EGizmoType GizmoType = EGizmoType::Translate;

	TMap<EAxis, UGizmoComponent*> GizmoComponents;
};

