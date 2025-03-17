#include "GizmoComponent.h"
#include <Object/Gizmo/GizmoActor.h>

UGizmoComponent::UGizmoComponent() : Super()
{
	// 메쉬 로드
	//VertexBuffer = FVertexBuffer::Find("Cube");
	//IndexBuffer = FIndexBuffer::Find("Cube");

	////없으면 만든다.
	//if (VertexBuffer == nullptr)
	//{
	//	TArray<FVertexSimple> vertices;
	//	TArray<uint32> indices;
	//	float size = 1.f;

	//	//UGeometryGenerator::CreateCube(size, &vertices, &indices);

	//	VertexBuffer = FVertexBuffer::Create(FString("Cube"), vertices);
	//	IndexBuffer = FIndexBuffer::Create(FString("Cube"), indices);
	//}
	//bCanBeRendered = true;
}

void UGizmoComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGizmoComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UGizmoComponent::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);
}

void UGizmoComponent::Render()
{
	Super::Render();
}

void UGizmoComponent::OnChangedGizmoType(EGizmoType Gizmo)
{
	// TODO : 이것들 변경
	// 
	////테스트 임시 메쉬
	//VertexBuffer;
	//IndexBuffer;
	//InputLayout;

	//// 테스트 임시 메테리얼
	//PixelShader;
	//VertexShader;

}
