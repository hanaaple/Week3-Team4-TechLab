#include "Sphere.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"


ASphere::ASphere()
{
	bCanEverTick = true;

	USphereComp* SphereComponent = AddComponent<USphereComp>();
	RootComponent = SphereComponent;
	
	SetActorTransform(FTransform());
}

void ASphere::BeginPlay()
{
	Super::BeginPlay();
}

void ASphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const FString ASphere::GetTypeName()
{
	return TEXT("Sphere");
}
