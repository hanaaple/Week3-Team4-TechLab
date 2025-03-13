#include "Camera.h"

#include "Core/Rendering/URenderer.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"
#include "Core/Input/PlayerInput.h"
#include "functional"

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
	APlayerInput::Get().RegisterKeyPressCallback(EKeyCode::Q, std::bind(&ACamera::MoveUp, this), GetUUID());
	APlayerInput::Get().RegisterKeyPressCallback(EKeyCode::E, std::bind(&ACamera::MoveDown, this), GetUUID());

	APlayerInput::Get().RegisterMouseDownCallback(EMouseButton::Right, std::bind(&ACamera::Rotate, this, std::placeholders::_1), GetUUID());
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
	//tr.Rotate(FVector(-mouseDelta.Y * 5.f, 0, mouseDelta.X * 5.f));

	SetActorTransform(tr);
}
