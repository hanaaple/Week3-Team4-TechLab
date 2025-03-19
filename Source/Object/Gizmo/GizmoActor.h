#pragma once
#include "Core/Interfaces/GizmoInterface.h"
#include "object/actor/actor.h"
#include "Object/PrimitiveComponent/GizmoComponent.h"


class AGizmoActor : public AActor, public IGizmoInterface
{
public:
	AGizmoActor();
	virtual ~AGizmoActor() = default;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//~ Begin IGizmoInterface
	virtual bool IsGizmo() override { return true; }
	//~ End IGizmoInterface

	void SetSelectedAxis(EAxis InAxis) { SelectedAxis = InAxis; }
	EAxis GetSelectedAxis() const { return SelectedAxis; }

	EGizmoType GetGizmoType() const { return GizmoType; }

private:
	EAxis SelectedAxis = EAxis::None;
	EGizmoType GizmoType = EGizmoType::Translate;

	TMap<EAxis, UGizmoComponent*> GizmoComponents;
};

