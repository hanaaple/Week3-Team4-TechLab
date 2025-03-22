#include "Quad.h"

#include "Object/PrimitiveComponent/StaticMeshComponent.h"

AQuad::AQuad()
{
	bCanEverTick = true;

	UQuadComp* QuadComponent = AddComponent<UQuadComp>();
	RootComponent = QuadComponent;

	QuadComponent->SetRelativeTransform(FTransform());
}

void AQuad::BeginPlay()
{
	Super::BeginPlay();
}

void AQuad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const FString AQuad::GetTypeName()
{
	return TEXT("Cube");
}
