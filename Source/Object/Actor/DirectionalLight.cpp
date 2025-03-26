#include "DirectionalLight.h"

#include "Object/Component/UDirectionalLightComp.h"


ADirectionalLight::ADirectionalLight()
{
	bCanEverTick = true;
}


void ADirectionalLight::BeginPlay()
{
	Super::BeginPlay();

	UDirectionalLightComp* DirectionalLightComp = AddComponent<UDirectionalLightComp>();
	RootComponent = DirectionalLightComp;

	SetActorRotation(FVector(1, 1, -1));
}

void ADirectionalLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const FString ADirectionalLight::GetTypeName()
{
	return TEXT("DirectionalLight");
}
