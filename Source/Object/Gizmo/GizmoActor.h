#pragma once
#include "Object/Actor/Actor.h"
#include "Object/PrimitiveComponent/GizmoComponent.h"


class AGizmoActor : public AActor
{
	DECLARE_CLASS(AGizmoActor, AActor)

public:
	AGizmoActor();
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

