#include "Camera.h"

#include "Core/Rendering/URenderer.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"
#include "Core/Input/PlayerInput.h"
#include "functional"
#include "Core/Config/ConfigManager.h"

ACamera::ACamera()
{
    bIsGizmo = true;
    
    Near = 1.f;
    Far = 100.f;
    FieldOfView = 45.f;
    ProjectionMode = ECameraProjectionMode::Perspective;
	CameraSpeed = 1.0f;
	Sensitivity = std::stof(UConfigManager::Get().GetValue("Camera", "Sensitivity"));

    RootComponent = AddComponent<USceneComponent>();
    
    FTransform StartPos = GetActorTransform();
    StartPos.SetPosition(FVector(-5, 0, 0));
    SetActorTransform(StartPos);
}

void ACamera::BeginPlay()
{
	Super::BeginPlay();
	APlayerInput::Get().RegisterKeyPressCallback(EKeyCode::W, [this] { MoveForward(); }, GetUUID());
	APlayerInput::Get().RegisterKeyPressCallback(EKeyCode::S, [this] { MoveBackward(); }, GetUUID());
	APlayerInput::Get().RegisterKeyPressCallback(EKeyCode::A, [this] { MoveLeft(); }, GetUUID());
	APlayerInput::Get().RegisterKeyPressCallback(EKeyCode::D, [this] { MoveRight(); }, GetUUID());
	APlayerInput::Get().RegisterKeyPressCallback(EKeyCode::Q, [this] { MoveDown(); }, GetUUID());
	APlayerInput::Get().RegisterKeyPressCallback(EKeyCode::E, [this] { MoveUp(); }, GetUUID());

	APlayerInput::Get().RegisterMousePressCallback(EKeyCode::RButton, std::bind(&ACamera::Rotate, this, std::placeholders::_1), GetUUID());

	UConfigManager::Get().SetValue("Camera", "Sensitivity", std::to_string(Sensitivity));
}

void ACamera::SetFieldOfVew(float Fov)
{
    FieldOfView = Fov;
}  

void ACamera::SetFar(float Far)
{
    this->Far = Far;
}

void ACamera::SetNear(float Near)
{
    this->Near = Near;
}

float ACamera::GetFieldOfView() const
{
    return  FieldOfView;
}

float ACamera::GetNear() const
{
    return Near;
}

float ACamera::GetFar() const
{
    return Far;
}

FMatrix ACamera::GetProjectionMatrix(float FrameBufferWidth, float FrameBufferHeight) const
{
	return FMatrix::PerspectiveFovLH(FieldOfView, FrameBufferWidth/FrameBufferHeight, Near, Far);
}

void ACamera::MoveForward()
{
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() + (GetForward() * CameraSpeed * UEngine::GetDeltaTime()));
	SetActorTransform(tr);
}

void ACamera::MoveBackward()
{
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() - (GetForward() * CameraSpeed * UEngine::GetDeltaTime()));
	SetActorTransform(tr);
}

void ACamera::MoveLeft()
{
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() - (GetRight() * CameraSpeed * UEngine::GetDeltaTime()));
	SetActorTransform(tr);
}

void ACamera::MoveRight()
{
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() + (GetRight() * CameraSpeed * UEngine::GetDeltaTime()));
	SetActorTransform(tr);
}

void ACamera::MoveUp()
{
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() + (FVector::UpVector * CameraSpeed * UEngine::GetDeltaTime()));
	SetActorTransform(tr);
}

void ACamera::MoveDown()
{
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() - (FVector::UpVector * CameraSpeed * UEngine::GetDeltaTime()));
	SetActorTransform(tr);
}

void ACamera::Rotate(const FVector& mouseDelta)
{
	FTransform tr = GetActorTransform();
	FVector TargetRotation = tr.GetRotation().GetEuler();
	TargetRotation.Y += Sensitivity * mouseDelta.Y;
	TargetRotation.Z += Sensitivity * mouseDelta.X;
	TargetRotation.Y = FMath::Clamp(TargetRotation.Y, -MaxYDegree, MaxYDegree);
	tr.SetRotation(TargetRotation);

	SetActorTransform(tr);
	//TargetRotation.Y = FMath::Clamp(TargetRotation.Y, -Camera->MaxYDegree, Camera->MaxYDegree);
	//CameraTransform.SetRotation(TargetRotation);


	//float CamSpeed = Camera->CameraSpeed;

	////회전이랑 마우스클릭 구현 카메라로 해야할듯?
	//CameraTransform.Translate(NewVelocity * DeltaTime * CamSpeed);
	//Camera->SetActorTransform(CameraTransform); //임시용
}
