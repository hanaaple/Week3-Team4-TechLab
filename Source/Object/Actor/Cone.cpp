#include "Cone.h"

#include "Object/PrimitiveComponent/StaticMeshComponent.h"


ACone::ACone()
{
    bCanEverTick = true;

    UConeComp* ConeComponent = AddComponent<UConeComp>();
    RootComponent = ConeComponent;
	
    SetActorTransform(FTransform());
}

void ACone::BeginPlay()
{
    Super::BeginPlay();
}

void ACone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

const FString ACone::GetTypeName()
{
    return TEXT("Cone");
}
