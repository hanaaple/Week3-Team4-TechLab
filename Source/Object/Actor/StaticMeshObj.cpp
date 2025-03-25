#include "StaticMeshObj.h"

#include "Object/PrimitiveComponent/StaticMeshComponent.h"

AStaticMeshObj::AStaticMeshObj()
{
	bCanEverTick = true;

	USphereComp* Comp = AddComponent<USphereComp>();
	RootComponent = Comp;
	Comp->SetRelativeTransform(FTransform());
	
	Comp->GetRenderResourceCollection().SetSamplerBinding("LinearSamplerState", 0, true, true); 
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
