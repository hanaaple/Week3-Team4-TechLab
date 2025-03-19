#pragma once
#include "Core/Interfaces/GizmoInterface.h"
#include "Object/Actor/Actor.h"


class AGizmoHandle : public AActor, public IGizmoInterface
{
	DECLARE_CLASS(AGizmoHandle, AActor)

public:
	AGizmoHandle();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void SetScaleByDistance();
	void SetActive(bool bActive);

	//~ Begin IGizmoInterface
	virtual bool IsGizmo() override { return true; }
	//~ End IGizmoInterface

	//void SetSelectedAxis(ESelectedAxis NewAxis) { SelectedAxis = NewAxis; }
//	ESelectedAxis GetSelectedAxis() const { return SelectedAxis; }
//	EGizmoType GetGizmoType() const { return GizmoType; }
//
//private:
//	bool bIsActive = false;
//	TArray<class UCylinderComp*> CylinderComponents;
//
//	ESelectedAxis SelectedAxis = ESelectedAxis::None;
//	EGizmoType GizmoType = EGizmoType::Translate;

	virtual const char* GetTypeName() override;

private:
	void DoTransform(FTransform& AT, FVector Result, AActor* Actor);
};

