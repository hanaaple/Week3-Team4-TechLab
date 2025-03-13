#include "UPrimitiveComponent.h"
#include "Object/World/World.h"
#include "Object/Actor/Actor.h"


void UPrimitiveComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPrimitiveComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
}

void UPrimitiveComponent::UpdateConstantPicking(const URenderer& Renderer, const FVector4 UUIDColor)const
{
	Renderer.UpdateConstantPicking(UUIDColor);
}

void UPrimitiveComponent::UpdateConstantDepth(const URenderer& Renderer, const int Depth)const
{
	Renderer.UpdateConstantDepth(Depth);
}

void UPrimitiveComponent::Render()
{
	URenderer* Renderer = UEngine::Get().GetRenderer();
	if (Renderer == nullptr || !bCanBeRendered)
	{
		return;
	}
	if (GetOwner()->IsGizmoActor() == false)
	{
		if (bIsPicked)
		{
			/*bUseVertexColor = false;
			SetCustomColor(FVector4(1.0f, 0.647f, 0.0f, 1.0f));*/
		}
		else
		{
			bUseVertexColor = true;
		}
	}

	//ACamera* cam = FEditorManager::Get().GetCamera();

	//// 객체와 카메라 위치 가져오기
	//FVector objectPosition = UpdateInfo.Transform.GetPosition();
	//FVector cameraPosition = cam->GetActorTransform().GetPosition();

	//FVector lookDir = (cameraPosition - objectPosition).GetSafeNormal();

	//// 언리얼 좌표계에 맞춘 구형 빌보드
	//// Z축이 상방 벡터
	//FVector up = FVector(0, 0, 1);

	//// Y축(우측)은 상방 벡터와 시선 방향의 외적
	//FVector right = up.Cross(lookDir).GetSafeNormal();

	//// X축(전방)은 우측 벡터와 상방 벡터의 외적
	//// 언리얼에서는 X가 전방이므로 이렇게 계산
	////FVector forward = right.Cross(up).GetSafeNormal();

	//up = lookDir.Cross(right).GetSafeNormal();

	//FMatrix result;

	//// 회전 행렬 구성
	//result.X = FVector4(lookDir.X, lookDir.Y, lookDir.Z, 0);
	//result.Y = FVector4(right.X, right.Y, right.Z, 0);
	//result.Z = FVector4(up.X, up.Y, up.Z, 0);
	//result.W = FVector4(0, 0, 0, 1);

	//FMatrix positionMatrix = FMatrix::GetTranslateMatrix(objectPosition);
	//FMatrix scaleMatrix = FMatrix::GetScaleMatrix(UpdateInfo.Transform.GetScale());


	//;

	//FMatrix MVP = FMatrix::Transpose(
	//	scaleMatrix *
	//	result *
	//	positionMatrix *
	//	ViewMatrix *
	//	ProjectionMatrix
	//);

	FMatrix ModelMatrix = CalculateModelMatrix();

	Renderer->RenderPrimitive(*this, ModelMatrix);
}

FMatrix UPrimitiveComponent::CalculateModelMatrix()
{
	return GetWorldTransform().GetMatrix();
}

void UPrimitiveComponent::RegisterComponentWithWorld(UWorld* World)
{
	World->AddRenderComponent(this);
}
