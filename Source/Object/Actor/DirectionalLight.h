#pragma once
#include "Actor.h"
#include "Core/Interfaces/GizmoInterface.h"

class ADirectionalLight : public AActor, public IGizmoInterface
{

	DECLARE_CLASS(ADirectionalLight, AActor)

public:
	ADirectionalLight();
	virtual bool IsGizmo() override { return true; }
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual const FString GetTypeName() override;

private:
};
