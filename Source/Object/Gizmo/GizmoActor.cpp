#include "GizmoActor.h"
#include <Object/PrimitiveComponent/UPrimitiveComponent.h>
#include "Core/Engine.h"
#include "Object/World/World.h"

AGizmoActor::AGizmoActor() : AActor()
{
	RootComponent = AddComponent<USceneComponent>();

	FTransform ZTransform;
	ZTransform.SetScale(FVector(0.1f, 0.1f, 1.0f));
	UCylinderComp* Z = AddComponent<UCylinderComp>(ZTransform);
	Z->SetCustomColor(FVector4::BLUE);

	FTransform YTransform;
	YTransform.SetScale(FVector(0.1f, 0.1f, 1.0f));
	YTransform.SetRotation(FVector(0.f, 0.f, 90.f));
	UCylinderComp* Y = AddComponent<UCylinderComp>(YTransform);
	Y->SetCustomColor(FVector4::GREEN);

	FTransform XTransform;
	XTransform.SetScale(FVector(0.1f, 0.1f, 1.0f));
	YTransform.SetRotation(FVector(0.f, 90.f, 0.f));
	UCylinderComp* X = AddComponent<UCylinderComp>(XTransform);
	X->SetCustomColor(FVector4::RED);

	UEngine::Get().GetWorld()->AddZIgnoreComponent(X);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(Y);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(Z);
}

void AGizmoActor::BeginPlay()
{
	AActor::BeginPlay();
}

void AGizmoActor::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
}
