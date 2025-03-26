#include "OrthoGraphicActor.h"

#include "Core/Input/PlayerInput.h"
#include "Core/Config/ConfigManager.h"
#include "Static/FEditorManager.h"
AOrthoGraphicActor::AOrthoGraphicActor()
{
	Near = .1f;
	Far = 1000.f;
	ZoomSize = 100.f;

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
	Offset.X = mouseDelta.Y * Sensitivity;
	Offset.Y = mouseDelta.X * Sensitivity;
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
	Offset.Z = mouseDelta.Y * Sensitivity;
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() + Offset);
	SetActorTransform(tr);
}

void AOrthoGraphicActor::MoveBack(const FVector& mouseDelta)
{
	FVector Offset;
	Offset.Y = -mouseDelta.X * Sensitivity;
	Offset.Z = mouseDelta.Y * Sensitivity;
	FTransform tr = GetActorTransform();
	tr.SetPosition(tr.GetPosition() + Offset);
	SetActorTransform(tr);
}
