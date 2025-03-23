#include "StaticMeshObj.h"

#include "Object/PrimitiveComponent/StaticMeshComponent.h"

AStaticMeshObj::AStaticMeshObj()
{
	bCanEverTick = true;

	USphereComp* Comp = AddComponent<USphereComp>();
	RootComponent = Comp;
	Comp->SetRelativeTransform(FTransform());
	Comp->SetMaterial("StaticMeshMaterial");
	Comp->GetRenderResourceCollection().SetSamplerBinding("LinearSamplerState", 0, true, true);

	Comp->GetRenderResourceCollection().SetTextureBinding("CubeTexture", 1, true, true);
	Comp->SetStaticMesh("CubeCube");
	
	// 신경 안써도 될듯?
	//Comp->GetRenderResourceCollection().SetConstantBufferBinding("SubUVVertexConstants", &GetVertexConstantsData(), 0, true, false);
	//Comp->GetRenderResourceCollection().SetConstantBufferBinding("SubUVPixelConstants", &GetPixelConstantsData(), 0, false, true);
}

void AStaticMeshObj::BeginPlay()
{
	Super::BeginPlay();
}

void AStaticMeshObj::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const FString AStaticMeshObj::GetTypeName()
{
	return TEXT("StaticMeshObj");
}
