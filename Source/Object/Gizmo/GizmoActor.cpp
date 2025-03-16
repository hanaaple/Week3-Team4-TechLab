#include "GizmoActor.h"
#include <Object/PrimitiveComponent/UPrimitiveComponent.h>
#include "Core/Engine.h"
#include "Object/World/World.h"

AGizmoActor::AGizmoActor() : AActor()
{
	RootComponent = AddComponent<USceneComponent>();
	RootComponent->SetRelativeTransform(FTransform({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }));

	UCylinderComp* Z = AddComponent<UCylinderComp>();
	Z->SetupAttachment(RootComponent);

	FTransform ZTransform = Z->GetComponentTransform();
	ZTransform.SetScale(FVector(0.1f, 0.1f, 1.0f));
	ZTransform.Rotate({ 0.0f, 0.0f, 0.0f });
	Z->SetRelativeTransform(ZTransform);
	Z->SetCustomColor(FVector4::BLUE);

	UCylinderComp* Y = AddComponent<UCylinderComp>();
	Y->SetupAttachment(RootComponent);

	FTransform YTransform = Y->GetComponentTransform();
	YTransform.SetScale(FVector(0.1f, 0.1f, 1.0f));
	YTransform.Rotate({ 0.0f, 0.0f, -90.0f });
	Y->SetRelativeTransform(YTransform);
	Y->SetCustomColor(FVector4::GREEN);

	UCylinderComp* X = AddComponent<UCylinderComp>();
	X->SetupAttachment(RootComponent);

	FTransform XTransform = X->GetComponentTransform();
	XTransform.SetScale(FVector(0.1f, 0.1f, 1.0f));
	XTransform.Rotate({ 0.0f, 90.0f, 0.0f });
	X->SetRelativeTransform(XTransform);
	X->SetCustomColor(FVector4::RED);


	FMatrix XWorldMat = X->GetWorldMatrix();
	FVector xpos = XWorldMat.GetTranslation();
	FVector xRot = XWorldMat.GetRotation();
	FVector xScale = XWorldMat.GetScale();

	FMatrix YWorldMat = Y->GetWorldMatrix();
	FVector ypos = YWorldMat.GetTranslation();
	FVector yRot = YWorldMat.GetRotation();
	FVector yScale = YWorldMat.GetScale();

	FMatrix ZWorldMat = Z->GetWorldMatrix();
	FVector zpos = ZWorldMat.GetTranslation();
	FVector zRot = ZWorldMat.GetRotation();
	FVector zScale = ZWorldMat.GetScale();


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
