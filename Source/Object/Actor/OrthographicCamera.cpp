#include "OrthoGraphicCamera.h"

AOrthoGraphicCamera::AOrthoGraphicCamera()
{
	Near = .1f;
	Far = 1000.f;
	ZoomSize = 1000.f;

	RootComponent = AddComponent<USceneComponent>();

	FTransform StartPos = GetActorTransform();
	StartPos.SetPosition(FVector(0, 0, 0));
	SetActorTransform(StartPos);
}

void AOrthoGraphicCamera::BeginPlay()
{
	Super::BeginPlay();
}

void AOrthoGraphicCamera::UpdateCameraMatrix()
{
	ViewMatrix = GetActorTransform().GetViewMatrix();

	float Near = GetNear();
	float Far = GetFar();

	ProjectionMatrix = FMatrix::OrthoForLH(UEngine::Get().GetScreenWidth() / ZoomSize, UEngine::Get().GetScreenHeight() / ZoomSize, Near, Far);

	ViewProjectionMatrix = ViewMatrix * ProjectionMatrix;
}

void AOrthoGraphicCamera::InitMatrix()
{
	ViewMatrix = FMatrix::Identity();
	ProjectionMatrix = FMatrix::Identity();
	ViewProjectionMatrix = FMatrix::Identity();
}

void AOrthoGraphicCamera::SetFar(float Far)
{
	this->Far = Far;
}

void AOrthoGraphicCamera::SetNear(float Near)
{
	this->Near = Near;
}

float AOrthoGraphicCamera::GetNear() const
{
	return Near;
}

float AOrthoGraphicCamera::GetFar() const
{
	return Far;
}

const FMatrix AOrthoGraphicCamera::GetTopViewMatrix() const
{
	return ViewProjectionMatrix * FMatrix::RotatePitch(90.f) * FMatrix::GetTranslateMatrix(FVector(0.f, 0.f, 10.f));
}

const FMatrix AOrthoGraphicCamera::GetBottomViewMatrix() const
{
	return ViewProjectionMatrix * FMatrix::RotatePitch(-90.f) * FMatrix::GetTranslateMatrix(FVector(0.f, 0.f, -10.f));
}

const FMatrix AOrthoGraphicCamera::GetLeftViewMatrix() const
{
	return ViewProjectionMatrix * FMatrix::RotateYaw(90.f) * FMatrix::GetTranslateMatrix(FVector(0.f, -10.f, 0.f));
}

const FMatrix AOrthoGraphicCamera::GetRightViewMatrix() const
{
	return ViewProjectionMatrix * FMatrix::RotateYaw(-90.f) * FMatrix::GetTranslateMatrix(FVector(0.f, 10.f, 0.f));
}

const FMatrix AOrthoGraphicCamera::GetFrontViewMatrix() const
{
	return ViewProjectionMatrix * FMatrix::RotateYaw(180.f) * FMatrix::GetTranslateMatrix(FVector(10.f, 0.f, 0.f));
}

const FMatrix AOrthoGraphicCamera::GetBackViewMatrix() const
{
	return ViewProjectionMatrix * FMatrix::RotateYaw(0.f)* FMatrix::GetTranslateMatrix(FVector(-10.f, 0.f, 0.f));
}
