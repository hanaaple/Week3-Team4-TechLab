#include "OrthoGraphicCamera.h"

#include "Core/Input/PlayerInput.h"
#include "Core/Config/ConfigManager.h"
#include "Static/FEditorManager.h"
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
	APlayerInput::Get().RegisterKeyPressCallback(EKeyCode::W, [this] { MoveForward(); }, GetUUID());
	APlayerInput::Get().RegisterKeyPressCallback(EKeyCode::S, [this] { MoveBackward(); }, GetUUID());
	APlayerInput::Get().RegisterKeyPressCallback(EKeyCode::A, [this] { MoveLeft(); }, GetUUID());
	APlayerInput::Get().RegisterKeyPressCallback(EKeyCode::D, [this] { MoveRight(); }, GetUUID());
	APlayerInput::Get().RegisterKeyPressCallback(EKeyCode::Q, [this] { MoveDown(); }, GetUUID());
	APlayerInput::Get().RegisterKeyPressCallback(EKeyCode::E, [this] { MoveUp(); }, GetUUID());
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

//const FMatrix AOrthoGraphicCamera::GetTopViewMatrix() const
//{
//	return ViewProjectionMatrix * FMatrix::RotatePitch(90.f) * FMatrix::GetTranslateMatrix(FVector(0.f, 0.f, 10.f));
//}
//
//const FMatrix AOrthoGraphicCamera::GetBottomViewMatrix() const
//{
//	return ViewProjectionMatrix * FMatrix::RotatePitch(-90.f) * FMatrix::GetTranslateMatrix(FVector(0.f, 0.f, -10.f));
//}
//
//const FMatrix AOrthoGraphicCamera::GetLeftViewMatrix() const
//{
//	return ViewProjectionMatrix * FMatrix::RotateYaw(90.f) * FMatrix::GetTranslateMatrix(FVector(0.f, -10.f, 0.f));
//}
//
//const FMatrix AOrthoGraphicCamera::GetRightViewMatrix() const
//{
//	return ViewProjectionMatrix * FMatrix::RotateYaw(-90.f) * FMatrix::GetTranslateMatrix(FVector(0.f, 10.f, 0.f));
//}
//
//const FMatrix AOrthoGraphicCamera::GetFrontViewMatrix() const
//{
//	return ViewProjectionMatrix * FMatrix::RotateYaw(180.f) * FMatrix::GetTranslateMatrix(FVector(10.f, 0.f, 0.f));
//}
//
//const FMatrix AOrthoGraphicCamera::GetBackViewMatrix() const
//{
//	return ViewProjectionMatrix * FMatrix::RotateYaw(0.f)* FMatrix::GetTranslateMatrix(FVector(-10.f, 0.f, 0.f));
//}

void AOrthoGraphicCamera::MoveForward()
{
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() + (GetForward() * CameraSpeed * UEngine::GetDeltaTime()));
	SetActorTransform(tr);
}

void AOrthoGraphicCamera::MoveBackward()
{
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() - (GetForward() * CameraSpeed * UEngine::GetDeltaTime()));
	SetActorTransform(tr);
}

void AOrthoGraphicCamera::MoveLeft()
{
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() - (GetRight() * CameraSpeed * UEngine::GetDeltaTime()));
	SetActorTransform(tr);
}

void AOrthoGraphicCamera::MoveRight()
{
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() + (GetRight() * CameraSpeed * UEngine::GetDeltaTime()));
	SetActorTransform(tr);
}

void AOrthoGraphicCamera::MoveUp()
{
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() + (FVector::UpVector * CameraSpeed * UEngine::GetDeltaTime()));
	SetActorTransform(tr);
}

void AOrthoGraphicCamera::MoveDown()
{
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() - (FVector::UpVector * CameraSpeed * UEngine::GetDeltaTime()));
	SetActorTransform(tr);
}

void AOrthoGraphicCamera::Rotate(const FVector& mouseDelta)
{
	FTransform tr = GetActorTransform();
	FVector TargetRotation = tr.GetRotation().GetEuler();
	TargetRotation.Y -= FMath::Clamp(Sensitivity * mouseDelta.Y, -MaxYDegree, MaxYDegree);
	TargetRotation.Z += Sensitivity * mouseDelta.X;
	tr.SetRotation(TargetRotation);

	SetActorTransform(tr);
	//TargetRotation.Y = FMath::Clamp(TargetRotation.Y, -Camera->MaxYDegree, Camera->MaxYDegree);
	//CameraTransform.SetRotation(TargetRotation);


	//float CamSpeed = Camera->CameraSpeed;

	////회전이랑 마우스클릭 구현 카메라로 해야할듯?
	//CameraTransform.Translate(NewVelocity * DeltaTime * CamSpeed);
	//Camera->SetActorTransform(CameraTransform); //임시용
}
