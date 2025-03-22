#pragma once
#include "Core/HAL/PlatformType.h"
#include "Core/Interfaces/GizmoInterface.h"
#include "Object/Actor/Actor.h"

class AOrthoGraphicCamera : public AActor, public IGizmoInterface
{
	DECLARE_CLASS(AOrthoGraphicCamera, AActor);

public:
	AOrthoGraphicCamera();

	virtual bool IsGizmo() override { return true; }

private:
	float Near;
	float Far;

	FMatrix ViewMatrix;
	FMatrix ProjectionMatrix;
	FMatrix ViewProjectionMatrix;

	float ZoomSize = 1000.f;
public:

	float CameraSpeed = 1.0f;
	float Sensitivity = 1.0f;

	virtual void BeginPlay() override;
	void UpdateCameraMatrix();
	void InitMatrix();

	void SetFar(float Far);
	void SetNear(float Near);

	float GetNear() const;
	float GetFar() const;

	const FMatrix GetTopViewMatrix() const;
	const FMatrix GetBottomViewMatrix() const;
	const FMatrix GetLeftViewMatrix() const;
	const FMatrix GetRightViewMatrix() const;
	const FMatrix GetFrontViewMatrix() const;
	const FMatrix GetBackViewMatrix() const;
};