#include "OrthoGraphicActor.h"

#include "Core/Input/PlayerInput.h"
#include "Core/Config/ConfigManager.h"
#include "Static/FEditorManager.h"
AOrthoGraphicActor::AOrthoGraphicActor()
{
	RootComponent = AddComponent<USceneComponent>();

	FTransform StartPos;
	StartPos.SetPosition(FVector(0, 0, 0));
	SetActorTransform(StartPos);
}

void AOrthoGraphicActor::BeginPlay()
{
	Super::BeginPlay();
}

void AOrthoGraphicActor::MoveTop(const FVector& mouseDelta)
{
	FVector Offset;
	Offset.X = -mouseDelta.Y * Sensitivity;
	Offset.Y = -mouseDelta.X * Sensitivity;
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() + Offset);
	SetActorTransform(tr);
}

void AOrthoGraphicActor::MoveBottom(const FVector& mouseDelta)
{
	FVector Offset;
	Offset.X = -mouseDelta.Y * Sensitivity;
	Offset.Y = mouseDelta.X * Sensitivity;
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() + Offset);
	SetActorTransform(tr);
}

void AOrthoGraphicActor::MoveLeft(const FVector& mouseDelta)
{
	FVector Offset;
	Offset.X = mouseDelta.X * Sensitivity;
	Offset.Z = -mouseDelta.Y * Sensitivity;
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() + Offset);
	SetActorTransform(tr);
}

void AOrthoGraphicActor::MoveRight(const FVector& mouseDelta)
{
	FVector Offset;
	Offset.X = -mouseDelta.X * Sensitivity;
	Offset.Z = -mouseDelta.Y * Sensitivity;
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() + Offset);
	SetActorTransform(tr);
}

void AOrthoGraphicActor::MoveFront(const FVector& mouseDelta)
{
	FVector Offset;
	Offset.Y = mouseDelta.X * Sensitivity;
	Offset.Z = -mouseDelta.Y * Sensitivity;
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() + Offset);
	SetActorTransform(tr);
}

void AOrthoGraphicActor::MoveBack(const FVector& mouseDelta)
{
	FVector Offset;
	Offset.Y = -mouseDelta.X * Sensitivity;
	Offset.Z = -mouseDelta.Y * Sensitivity;
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() + Offset);
	SetActorTransform(tr);
}

FTransform AOrthoGraphicActor::GetTransform(ELevelViewportType InLevelViewportType)
{
	FTransform Top;
	Top.SetPosition(GetActorPosition() + FVector(0.f, 0.f, 50.f));
	Top.SetRotation(FVector(0.f, 89.9f, 0.f));

	FTransform Bottom;
	Bottom.SetPosition(GetActorPosition() + FVector(0.f, 0.f, -50.f));
	Bottom.SetRotation(FVector(0.f, -89.9f, 180.f));

	FTransform Left;
	Left.SetPosition(GetActorPosition() + FVector(0.f, -50.f, 0.f));
	Left.SetRotation(FVector(0.f, 0.f, 90.f));

	FTransform Right;
	Right.SetPosition(GetActorPosition() + FVector(0.f, 50.f, 0.f));
	Right.SetRotation(FVector(0.f, 0.f, -90.f));

	FTransform Front;
	Front.SetPosition(GetActorPosition() + FVector(50.f, 0.f, 0.f));
	Front.SetRotation(FVector(0.f, 0.f, 180.f));

	FTransform Back;
	Back.SetPosition(GetActorPosition() + FVector(-0.f, 0.f, 0.f));
	Back.SetRotation(FVector(0.f, 0.f, 0.f));

	switch (InLevelViewportType)
	{
	case ELevelViewportType::Top:
		return Top;
	case ELevelViewportType::Bottom:
		return Bottom;
	case ELevelViewportType::Left:
		return Left;
	case ELevelViewportType::Right:
		return Right;
	case ELevelViewportType::Front:
		return Front;
	case ELevelViewportType::Back:
		return Back;
	}
}