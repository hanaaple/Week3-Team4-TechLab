#pragma once
#include "Core/HAL/PlatformType.h"
#include "Core/Interfaces/GizmoInterface.h"
#include "Object/Actor/Actor.h"

class AOrthoGraphicActor : public AActor, public IGizmoInterface
{
	DECLARE_CLASS(AOrthoGraphicActor, AActor);

public:
	AOrthoGraphicActor();

	virtual bool IsGizmo() override { return true; }

public:
	float Sensitivity = 5.0f;

	virtual void BeginPlay() override;
	void MoveTop(const FVector& mouseDelta);
	void MoveBottom(const FVector& mouseDelta);
	void MoveLeft(const FVector& mouseDelta);
	void MoveRight(const FVector& mouseDelta);
	void MoveFront(const FVector& mouseDelta);
	void MoveBack(const FVector& mouseDelta);
};