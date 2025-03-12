#include "Camera.h"

#include "Core/Rendering/URenderer.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"
#include "Core/Input/PlayerInput.h"


ACamera::ACamera()
{
    bIsGizmo = true;
    
    Near = 0.1f;
    Far = 100.f;
    FieldOfView = 45.f;
    ProjectionMode = ECameraProjectionMode::Perspective;
	CameraSpeed = 1.0f;

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
	FTransform StartPos = GetActorTransform();
	StartPos.SetPosition(StartPos.GetPosition() + GetForward() * CameraSpeed);

	SetActorTransform(StartPos);
}

void ACamera::MoveBackward()
{
	FTransform StartPos = GetActorTransform();
	StartPos.SetPosition(StartPos.GetPosition() - GetForward() * CameraSpeed);

	SetActorTransform(StartPos);
}

void ACamera::MoveLeft()
{
	FTransform StartPos = GetActorTransform();
	StartPos.SetPosition(StartPos.GetPosition() - GetRight() * CameraSpeed);

	SetActorTransform(StartPos);
}

void ACamera::MoveRight()
{
	FTransform StartPos = GetActorTransform();
	StartPos.SetPosition(StartPos.GetPosition() + GetRight() * CameraSpeed);

	SetActorTransform(StartPos);
}
