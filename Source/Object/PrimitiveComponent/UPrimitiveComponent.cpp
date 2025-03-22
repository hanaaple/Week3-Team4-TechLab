#include "UPrimitiveComponent.h"

#include "Core/Rendering/URenderer.h"
#include "Debug/EngineShowFlags.h"
#include "Object/Actor/Actor.h"
#include "Object/Actor/Camera.h"
#include "Object/World/World.h"
#include "Resource/StaticMesh.h"
#include "Resource/DirectResource/BlendState.h"
#include "Resource/DirectResource/Vertexbuffer.h"
#include "Static/FEditorManager.h"


UPrimitiveComponent::UPrimitiveComponent()
{
	bCanBeRendered = true;

	// 기본으로 바인딩되는 데이타
	GetRenderResourceCollection().SetConstantBufferBinding(TEXT("FConstantsComponentData"), &ConstantsComponentData, 0, true, true);
	SetMaterial("DefaultMaterial");
}

UPrimitiveComponent::~UPrimitiveComponent()
{
}

void UPrimitiveComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPrimitiveComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
	UpdateBounds();
}

// void UPrimitiveComponent::UpdateConstantPicking(const URenderer& Renderer, const FVector4 UUIDColor)const
// {
// 	Renderer.UpdateConstantPicking(UUIDColor);
// }

// void UPrimitiveComponent::UpdateConstantDepth(const URenderer& Renderer, const int Depth)const
// {
// 	Renderer.UpdateConstantDepth(Depth);
// }

void UPrimitiveComponent::Render()
{
	URenderer* Renderer = UEngine::Get().GetRenderer();
	if (Renderer == nullptr || !FEngineShowFlags::Get().GetSingleFlag(EEngineShowFlags::SF_Primitives) || !bCanBeRendered)
	{
		return;
	}
	// if (GetOwner()->Implements<IGizmoInterface>() == false) // TODO: RTTI 개선하면 사용
	if (!dynamic_cast<IGizmoInterface*>(GetOwner()))
	{
		if (bIsPicked)
		{
			bUseVertexColor = false;
			SetCustomColor(FVector4(1.0f, 0.647f, 0.0f, 1.0f));
		}
		else
		{
			bUseVertexColor = true;
		}
	}

	FMatrix ModelMatrix;
	CalculateModelMatrix(ModelMatrix);

	const FMatrix& ViewProjectionMatrix = UEngine::Get().GetWorld()->GetCamera()->GetViewProjectionMatrix();

	FMatrix MVP = FMatrix::Transpose(ModelMatrix * ViewProjectionMatrix);
	
	uint32 ID = GetUUID();

	FVector4 UUIDCOlor = FEditorManager::EncodeUUID(ID);

	FConstantsComponentData& Data = GetConstantsComponentData();

	Data = {
		.MVP = MVP,
		.Color = GetCustomColor(),
		.UUIDColor = UUIDCOlor,
		.bUseVertexColor = IsUseVertexColor()
	};
	

	Renderer->Render(GetRenderResourceCollection());
}

void UPrimitiveComponent::CalculateModelMatrix(FMatrix& OutMatrix)
{
	//빌보드 행렬계산
	if (bIsBillboard == true)
	{
		ACamera* cam = UEngine::Get().GetWorld()->GetCamera();

		FVector cameraPosition = cam->GetActorTransform().GetPosition();


		FVector objectPosition = WorldTransform.GetPosition();	
		FVector objectScale = WorldTransform.GetScale();

		FVector lookDir = (cameraPosition - objectPosition).GetSafeNormal();

		// 언리얼 좌표계에 맞춘 구형 빌보드, Z축이 상방 벡터
		// Y축(우측)은 상방 벡터와 시선 방향의 외적
		FVector right = FVector(0, 0, 1).Cross(lookDir).GetSafeNormal();

		FVector up = lookDir.Cross(right).GetSafeNormal();

		// X축(전방)은 우측 벡터와 상방 벡터의 외적
		// 언리얼에서는 X가 전방이므로 이렇게 계산
		//FVector forward = right.Cross(up).GetSafeNormal();

		FMatrix rotationMatrix;

		// 회전 행렬 구성
		rotationMatrix.X = FVector4(lookDir.X, lookDir.Y, lookDir.Z, 0);
		rotationMatrix.Y = FVector4(right.X, right.Y, right.Z, 0);
		rotationMatrix.Z = FVector4(up.X, up.Y, up.Z, 0);
		rotationMatrix.W = FVector4(0, 0, 0, 1);

		FMatrix positionMatrix = FMatrix::GetTranslateMatrix(objectPosition);
		FMatrix scaleMatrix = FMatrix::GetScaleMatrix(objectScale);

		OutMatrix = scaleMatrix * rotationMatrix * positionMatrix;

		return;
	}
	OutMatrix = WorldTransform.GetMatrix();
	return;
}

void UPrimitiveComponent::RegisterComponentWithWorld(UWorld* World)
{
	World->AddRenderComponent(this);
}

void UPrimitiveComponent::SetBoundsScale(float NewBoundScale)
{
	BoundsScale = NewBoundScale;
	UpdateBounds();
}

void UPrimitiveComponent::SetBoxExtent(const FVector& InExtent)
{
	BoxExtent = InExtent;
	UpdateBounds();
}

FBoxSphereBounds UPrimitiveComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	return FBoxSphereBounds(FBox(-BoxExtent, BoxExtent)).TransformBy(LocalToWorld);
}