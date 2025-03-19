#include "SubUVParticle.h"
#include "Object/SubUVComponent/UParticleSubUVComponent.h"

ASubUVParticle::ASubUVParticle()
{
	bCanEverTick = true;

	UParticleSubUVComponent* SubUVComponent = AddComponent<UParticleSubUVComponent>();
	RootComponent = SubUVComponent;

	SubUVComponent->SetRelativeTransform(FTransform());
}

void ASubUVParticle::BeginPlay()
{
	Super::BeginPlay();
}

void ASubUVParticle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}