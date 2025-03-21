#include "SpotLight.h"
#include "Object/Light/SpotLightComponent.h"

ASpotLight::ASpotLight()
{
	bCanEverTick = true;

	USpotLightComponent* SpotLightComponent = AddComponent<USpotLightComponent>();
	RootComponent = SpotLightComponent;

	SetActorTransform(FTransform());
}

void ASpotLight::BeginPlay()
{
	Super::BeginPlay();
}

void ASpotLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const FString ASpotLight::GetTypeName()
{
	return TEXT("SpotLight");
}
