#include "StaticMeshObj.h"

#include "Object/PrimitiveComponent/StaticMeshComponent.h"

AStaticMeshObj::AStaticMeshObj()
{
	bCanEverTick = true;

	USphereComp* Comp = AddComponent<USphereComp>();
	RootComponent = Comp;
	Comp->SetRelativeTransform(FTransform());
	Comp->SetMaterial("StaticMeshMaterial");


	// Material에 Texture, Shader를 합친다

	// Mesh 변경 시 Mesh에 연결된 Material을 변경한다.
	// 그렇지만 Material 변경이 가능은 하다.

	
	// SetStaticMesh("Cube") -> SetMaterial("")      이후 변경된 Material이 있다면 캐싱을 해두지 말자 일단. ㅇㅇ

	// 따로 SetTE
	
		
//	Comp->GetRenderResourceCollection().SetSamplerBinding("LinearSamplerState", 0, true, true);
	//Comp->GetRenderResourceCollection().SetTextureBinding("CubeTexture", 1, true, true);
	//Comp->SetStaticMesh("CubeCube");


	
	
	
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
