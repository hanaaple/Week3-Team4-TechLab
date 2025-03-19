#include "UPrimitiveComponent.h"
#include "Object/World/World.h"
#include "Object/Actor/Actor.h"
#include "Core/Engine.h"
#include "Object/Actor/Camera.h"
#include "Primitive/UGeometryGenerator.h"
#include "Resource/DirectResource/Vertexbuffer.h"
#include "Resource/DirectResource/VertexShader.h"
#include "Resource/DirectResource/PixelShader.h"
#include "Resource/DirectResource/InputLayout.h"
#include "Resource/DirectResource/ConstantBuffer.h"
#include "Debug/EngineShowFlags.h"
#include "Resource/DirectResource/BlendState.h"
#include "Resource/DirectResource/DepthStencilState.h"
#include "Resource/DirectResource/Rasterizer.h"
#include "Resource/DirectResource/ShaderResourceBinding.h"
#include "Static/FEditorManager.h"

UPrimitiveComponent::UPrimitiveComponent() : Super()
{
	bCanBeRendered = true;


	// 기본으로 바인딩되는 데이타
	GetRenderResourceCollection().SetConstantBufferBinding("FConstantsComponentData", &ConstantsComponentData, 0, true, true);
	SetMaterial("DefaultMaterial");
}

UPrimitiveComponent::~UPrimitiveComponent()
{
	class std::shared_ptr<class FVertexBuffer> VertexBuffer = nullptr;
	class std::shared_ptr<class FIndexBuffer> IndexBuffer = nullptr;
}

void UPrimitiveComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPrimitiveComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
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
	if (GetOwner()->IsGizmoActor() == false)
	{
		if (bIsPicked)
		{
			bUseVertexColor = true;
			/*bUseVertexColor = false;
			SetCustomColor(FVector4(1.0f, 0.647f, 0.0f, 1.0f));*/
		}
		else
		{
			bUseVertexColor = true;
		}
	}

	FMatrix ModelMatrix;
	CalculateModelMatrix(ModelMatrix);

	const FMatrix& ViewProjectionMatrix = UEngine::Get().GetWorld()->GetCamera()->GetViewProjectionMatrix();

	FMatrix MVP = FMatrix::Transpose(
		ModelMatrix *
		ViewProjectionMatrix
);
	
	uint32 ID = GetOwner()->GetUUID();

	FVector4 UUIDCOlor = FEditorManager::EncodeUUID(ID);

	FConstantsComponentData& Data = GetConstantsComponentData();

	Data  = {
		MVP,
		GetCustomColor(),
		UUIDCOlor,
		IsUseVertexColor()
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


		FVector objectPosition = GetWorldTransform().GetPosition();	
		FVector objectScale = GetWorldTransform().GetScale();

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
	OutMatrix = GetWorldTransform().GetMatrix();
	return;
}

void UPrimitiveComponent::RegisterComponentWithWorld(UWorld* World)
{
	World->AddRenderComponent(this);
}

UCubeComp::UCubeComp() : Super()
{

	SetMesh("Cube");
	bCanBeRendered = true;
}

USphereComp::USphereComp() : Super()
{
	SetMesh("Sphere");
	bCanBeRendered = true;
}

UTriangleComp::UTriangleComp() : Super()
{
	
	SetMesh("Triangle");
	bCanBeRendered = true;
}

UQuadComp::UQuadComp()
{
	SetMesh("Quad");
}

ULineComp::ULineComp() : Super()
{
	SetMesh("Line");
}

UCylinderComp::UCylinderComp() : Super()
{
	SetMesh("Cylinder");
}

UConeComp::UConeComp() : Super()
{
	SetMesh("Cone");
}
