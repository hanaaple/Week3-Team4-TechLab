#include "GizmoActor.h"
#include <Object/PrimitiveComponent/UPrimitiveComponent.h>
#include "Core/Engine.h"
#include "Object/World/World.h"

AGizmoActor::AGizmoActor() : AActor()
{
	bCanEverTick = true;

	RootComponent = AddComponent<USceneComponent>();

	UGizmoComponent* ZGizmo = AddComponent<UGizmoComponent>(FTransform(FVector(0.0f, 0.0f, 0.0f), FVector(0, 0, 0), FVector(0.1, 0.1, 2)));
	ZGizmo->SetCustomColor(FVector4::BLUE);
	GizmoComponents.Add(EAxis::Z, ZGizmo);

	UGizmoComponent* YGizmo = AddComponent<UGizmoComponent>(FTransform(FVector(0.0f, 0.0f, 0.0f), FVector(90, 0, 0), FVector(0.1, 0.1, 2)));
	YGizmo->SetCustomColor(FVector4::GREEN);
	GizmoComponents.Add(EAxis::Y, YGizmo);

	UGizmoComponent* XGizmo = AddComponent<UGizmoComponent>(FTransform(FVector(0.0f, 0.0f, 0.0f), FVector(0, 90, 0), FVector(0.1, 0.1, 2)));
	XGizmo->SetCustomColor(FVector4::RED);
	GizmoComponents.Add(EAxis::X, XGizmo);

	UEngine::Get().GetWorld()->AddZIgnoreComponent(XGizmo);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(YGizmo);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(ZGizmo);
}

void AGizmoActor::BeginPlay()
{
	AActor::BeginPlay();
}

void AGizmoActor::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
}
