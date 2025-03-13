#include "Camera.h"

#include "Core/Rendering/URenderer.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"
#include "Core/Input/PlayerInput.h"
#include "functional"
#include "Core/Config/ConfigManager.h"

ACamera::ACamera()
{
    bIsGizmo = true;
    
    Near = 0.1f;
    Far = 1000.f;
    FieldOfView = 45.f;
    ProjectionMode = ECameraProjectionMode::Perspective;
	CameraSpeed = 20.0f;
	float sensitivity = std::stof(UConfigManager::Get().GetValue("Camera", "Sensitivity"));

    RootComponent = AddComponent<USceneComponent>();
    
    FTransform StartPos = GetActorTransform();
    StartPos.SetPosition(FVector(-5, 0, 0));
    SetActorTransform(StartPos);
}

void ACamera::BeginPlay()
{
	Super::BeginPlay();
	APlayerInput::Get().RegisterKeyPressCallback(EKeyCode::W, std::bind(&ACamera::MoveForward, this), GetUUID());
	APlayerInput::Get().RegisterKeyPressCallback(EKeyCode::S, std::bind(&ACamera::MoveBackward, this), GetUUID());
	APlayerInput::Get().RegisterKeyPressCallback(EKeyCode::A, std::bind(&ACamera::MoveLeft, this), GetUUID());
	APlayerInput::Get().RegisterKeyPressCallback(EKeyCode::D, std::bind(&ACamera::MoveRight, this), GetUUID());
	APlayerInput::Get().RegisterKeyPressCallback(EKeyCode::Q, std::bind(&ACamera::MoveUp, this), GetUUID());
	APlayerInput::Get().RegisterKeyPressCallback(EKeyCode::E, std::bind(&ACamera::MoveDown, this), GetUUID());

	///APlayerInput::Get().RegisterMousePressCallback(EKeyCode::RButton, std::bind(&ACamera::Rotate, this, std::placeholders::_1), GetUUID());

	UConfigManager::Get().SetValue("Camera", "Sensitivity", std::to_string(20));
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

void ACamera::MoveForward()
{
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() + GetForward() * CameraSpeed);
	SetActorTransform(tr);
}

void ACamera::MoveBackward()
{
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() - GetForward() * CameraSpeed);
	SetActorTransform(tr);
}

void ACamera::MoveLeft()
{
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() - GetRight() * CameraSpeed);
	SetActorTransform(tr);
}

void ACamera::MoveRight()
{
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() + GetRight() * CameraSpeed);
	SetActorTransform(tr);
}

void ACamera::MoveUp()
{
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() + GetUp() * CameraSpeed);
	SetActorTransform(tr);
}

void ACamera::MoveDown()
{
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() - GetUp() * CameraSpeed);
	SetActorTransform(tr);
}

void ACamera::Rotate(const FVector& mouseDelta)
{
	FTransform tr = GetActorTransform();
	FVector TargetRotation = tr.GetRotation().GetEuler();
	TargetRotation.Y += CameraSpeed * mouseDelta.Y;
	TargetRotation.Z += CameraSpeed * mouseDelta.X;
	TargetRotation.Y = FMath::Clamp(TargetRotation.Y, -MaxYDegree, MaxYDegree);
	tr.SetRotation(TargetRotation);
	tr.Rotate(FVector(-mouseDelta.X, - mouseDelta.Y, 0));

	SetActorTransform(tr);
}
