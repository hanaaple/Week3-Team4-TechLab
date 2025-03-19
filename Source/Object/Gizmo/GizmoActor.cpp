#include "GizmoActor.h"
#include <Object/PrimitiveComponent/UPrimitiveComponent.h>
#include "Core/Engine.h"
#include "Object/World/World.h"
#include "Static/FEditorManager.h"

#include "Object/Actor/Camera.h"
#include "Core/Input/PlayerInput.h"

AGizmoActor::AGizmoActor() : AActor()
{
	bCanEverTick = true;

	RootComponent = AddComponent<USceneComponent>();

	UGizmoComponent* ZGizmo = AddComponent<UGizmoComponent>();
	ZGizmo->SetRelativeTransform({
		FVector(0.0f, 0.0f, 0.0f),
		FVector(0.0f, 0.0f, 0.0f),
		FVector(1.0f)
	});
	ZGizmo->SetCustomColor(FVector4::BLUE * 0.75f);
	ZGizmo->Axis = ESelectedAxis::Z;

	GizmoComponents.Add(ESelectedAxis::Z, ZGizmo);

	UGizmoComponent* YGizmo = AddComponent<UGizmoComponent>();
	YGizmo->SetRelativeTransform({
		FVector(0.0f, 0.0f, 0.0f),
		FVector(90.0f, 0.0f, 0.0f),
		FVector(1.0f)
	});

	YGizmo->SetCustomColor(FVector4::GREEN * 0.75f);

	YGizmo->Axis = ESelectedAxis::Y;

	GizmoComponents.Add(ESelectedAxis::Y, YGizmo);


	UGizmoComponent* XGizmo = AddComponent<UGizmoComponent>();
	XGizmo->SetRelativeTransform({
		FVector(0.0f, 0.0f, 0.0f),
		FVector(0, 90, 0),
		FVector(1.0f)
	});
	XGizmo->SetCustomColor(FVector4::RED * 0.757f);
	GizmoComponents.Add(ESelectedAxis::X, XGizmo);
	XGizmo->Axis = ESelectedAxis::X;

	USphereComp* Comp = AddComponent<USphereComp>();

	Comp->SetMaterial(TEXT("AlwaysVisibleMaterial"));
	//Comp->SetMesh(TEXT("GizmoArrow"));
	Comp->SetRelativeTransform({
	FVector(0.0f, 0.0f, 0.0f),
	FVector(0, 0, 0),
	FVector(0.2f)
		});

	FVector4 Color = FVector4::WHITE * 0.8f;
	Color.W = 1.0f;
	Comp->SetCustomColor(Color);


	UEngine::Get().GetWorld()->AddZIgnoreComponent(XGizmo);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(YGizmo);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(ZGizmo);
	UEngine::Get().GetWorld()->AddZIgnoreComponent(Comp);
}

void AGizmoActor::BeginPlay()
{
	AActor::BeginPlay();
}

void AGizmoActor::SetScaleByDistance()
{
	FTransform MyTransform = GetActorTransform();

	// 액터의 월드 위치
	FVector actorWorldPos = MyTransform.GetPosition();

	FTransform CameraTransform = FEditorManager::Get().GetCamera()->GetActorTransform();

	// 카메라의 월드 위치
	FVector cameraWorldPos = CameraTransform.GetPosition();

	// 거리 계산
	float distance = (actorWorldPos - cameraWorldPos).Length();

	float baseScale = 1.0f;    // 기본 스케일
	float scaleFactor = distance * 0.1f; // 거리에 비례하여 스케일 증가

	// float minScale = 1.0f;     // 최소 스케일
	// float maxScale = 1.0f;     // 최대 스케일
	// float scaleFactor = clamp(1.0f / distance, minScale, maxScale);

	MyTransform.SetScale(scaleFactor, scaleFactor, scaleFactor);
}



void AGizmoActor::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	SetScaleByDistance();

	if (SelectedAxis != ESelectedAxis::None and IsPicked() == true and APlayerInput::Get().GetKeyPress(EKeyCode::LButton))
	{
		if (AActor* Actor = FEditorManager::Get().GetSelectedActor())
		{
			// 마우스의 커서 위치를 가져오기
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(UEngine::Get().GetWindowHandle(), &pt);

			RECT Rect;
			GetClientRect(UEngine::Get().GetWindowHandle(), &Rect);
			int ScreenWidth = Rect.right - Rect.left;
			int ScreenHeight = Rect.bottom - Rect.top;

			// 커서 위치를 NDC로 변경
			float PosX = 2.0f * pt.x / ScreenWidth - 1.0f;
			float PosY = -2.0f * pt.y / ScreenHeight + 1.0f;

			// Projection 공간으로 변환
			FVector4 RayOrigin{ PosX, PosY, 0.0f, 1.0f };
			FVector4 RayEnd{ PosX, PosY, 1.0f, 1.0f };

			// View 공간으로 변환
			FMatrix InvProjMat = UEngine::Get().GetWorld()->GetCamera()->GetProjectionMatrix().Inverse();
			RayOrigin = InvProjMat.TransformVector4(RayOrigin);
			RayOrigin.W = 1;
			RayEnd = InvProjMat.TransformVector4(RayEnd);
			RayEnd *= UEngine::Get().GetWorld()->GetCamera()->GetFar();  // 프러스텀의 Far 값이 적용이 안돼서 수동으로 곱함
			RayEnd.W = 1;

			// 마우스 포인터의 월드 위치와 방향
			FMatrix InvViewMat = FEditorManager::Get().GetCamera()->GetViewMatrix().Inverse();
			RayOrigin = InvViewMat.TransformVector4(RayOrigin);
			RayOrigin /= RayOrigin.W = 1;
			RayEnd = InvViewMat.TransformVector4(RayEnd);
			RayEnd /= RayEnd.W = 1;
			FVector RayDir = (RayEnd - RayOrigin).GetSafeNormal();

			// 액터와의 거리
			float Distance = FVector::Distance(RayOrigin, Actor->GetActorTransform().GetPosition());

			// Ray 방향으로 Distance만큼 재계산


			FVector Result = RayDir * Distance;
			Result += RayOrigin;

			// 액터의 월드 위
			FTransform AT = Actor->GetActorTransform();

			DoTransform(AT, Result, Actor);

		}
	}
	
	if (SelectedAxis != ESelectedAxis::None and APlayerInput::Get().GetKeyDown(EKeyCode::Space))
	{
		// 현재 GizmoType을 가져옴 (CurrentGizmoType이 현재 타입을 저장하는 변수라고 가정)
		EGizmoType& CurrentGizmoType = GizmoType; // 이 부분은 실제 구현에 맞게 수정해야 함

		// 다음 타입으로 순환
		int nextType = static_cast<int>(CurrentGizmoType) + 1;

		// Max에 도달하면 다시 처음(Translate)으로 돌아감
		if (nextType >= static_cast<int>(EGizmoType::Max))
		{
			nextType = static_cast<int>(EGizmoType::Translate);
		}

		// 새 GizmoType 적용
		GizmoType = static_cast<EGizmoType>(nextType);

		for (auto iter: GizmoComponents)
		{
			iter.Value->OnChangedGizmoType(GizmoType);
		}
	}
}

void AGizmoActor::DoTransform(FTransform& AT, FVector Result, AActor* Actor)
{
	const FVector& AP = AT.GetPosition();

 	if (SelectedAxis == ESelectedAxis::X)
 	{
 		switch (GizmoType)
 		{
 		case EGizmoType::Translate:
 			AT.SetPosition({ Result.X, AP.Y, AP.Z });
 			break;
 		case EGizmoType::Rotate:
 			AT.RotatePitch(Result.X);
 			break;
 		case EGizmoType::Scale:
 			AT.AddScale({ Result.X * .1f, 0, AP.Z * .1f });
 			break;
 		}
 	}
 	else if (SelectedAxis == ESelectedAxis::Y)
 	{
 		switch (GizmoType)
 		{
 		case EGizmoType::Translate:
 			AT.SetPosition({ AP.X, Result.Y, AP.Z });
 			break;
 		case EGizmoType::Rotate:
 			AT.RotateRoll(Result.Y);
 			break;
 		case EGizmoType::Scale:
 			AT.AddScale({ 0, Result.Y * .1f, 0 });
 			break;
 		}
 	}
 	else if (SelectedAxis == ESelectedAxis::Z)
 	{
 		switch (GizmoType)
 		{
 		case EGizmoType::Translate:
 			AT.SetPosition({ AP.X, AP.Y, Result.Z });
 			break;
 		case EGizmoType::Rotate:
 			AT.RotatePitch(-Result.Z);
 			break;
 		case EGizmoType::Scale:
 			AT.AddScale({0, 0, Result.Z * .1f });
 			break;
 		}
 	}
 	Actor->SetActorTransform(AT);

}

