#include "GizmoActor.h"
#include <Object/PrimitiveComponent/UPrimitiveComponent.h>
#include "Core/Engine.h"
#include "Object/World/World.h"

AGizmoActor::AGizmoActor() : AActor()
{
	bCanEverTick = true;

	RootComponent = AddComponent<USceneComponent>();

	UCylinderComp* Z = AddComponent<UCylinderComp>(FTransform(FVector(0.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f), FVector(0.1f, 0.1f, 2)));
	Z->SetCustomColor(FVector4::BLUE);

	UCylinderComp* Y = AddComponent<UCylinderComp>(FTransform(FVector(0.0f, 0.0f, 0.0f), FVector(90.0f, 0.0f, 0.0f), FVector(0.1f, 0.1f, 2)));
	Y->SetCustomColor(FVector4::GREEN);

	UCylinderComp* X = AddComponent<UCylinderComp>(FTransform(FVector(0.0f, 0.0f, 0.0f), FVector(0.0f, 90.0f, 0.0f), FVector(0.1f, 0.1f, 2)));
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
