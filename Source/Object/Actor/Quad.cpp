#include "Quad.h"
#include <Object/PrimitiveComponent/UPrimitiveComponent.h>

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

const char* AQuad::GetTypeName()
{
	return "Cube";
}
