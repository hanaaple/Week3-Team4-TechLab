#include "Cylinder.h"

#include <Object/PrimitiveComponent/UPrimitiveComponent.h>

ACylinder::ACylinder()
{
    bCanEverTick = true;

    UCylinderComp* CylinderComponent = AddComponent<UCylinderComp>();
	
	UCylinderComp* CylinderComponent2 = AddComponent<UCylinderComp>(FTransform(FVector(0.0f, 0.0f, 0.0f), FVector(90, 0, 0), FVector(1, 1, 1)));

	UCylinderComp* CylinderComponent3 = AddComponent<UCylinderComp>(FTransform(FVector(0.0f, 0.0f, 0.0f), FVector(0, 90, 0), FVector(1, 1, 1)));

    SetActorTransform(FTransform());
}

void ACylinder::BeginPlay()
{
    Super::BeginPlay();
}

void ACylinder::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

const char* ACylinder::GetTypeName()
{
    return "Cylinder";
}
