#pragma once
#include "Object/Actor/Actor.h"


class AGizmoHandle : public AActor
{
	DECLARE_CLASS(AGizmoHandle, AActor)

public:
	AGizmoHandle();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void SetScaleByDistance();
	void SetActive(bool bActive);
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

