#pragma once
#include "Core/Interfaces/GizmoInterface.h"
#include "Object/Actor/Actor.h"



class AAxis : public AActor, public IGizmoInterface
{
	DECLARE_CLASS(AAxis, AActor)

public:
	AAxis();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual const char* GetTypeName() override;

	//~ Begin IGizmoInterface
	virtual bool IsGizmo() override { return true; }
	//~ End IGizmoInterface
};

