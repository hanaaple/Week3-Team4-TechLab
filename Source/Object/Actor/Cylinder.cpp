#include "Cylinder.h"

#include <Object/PrimitiveComponent/UPrimitiveComponent.h>

ACylinder::ACylinder()
{
    bCanEverTick = true;

    UCylinderComp* CylinderComponent = AddComponent<UCylinderComp>();
    RootComponent = CylinderComponent;
	
	UCylinderComp* CylinderComponent2 = AddComponent<UCylinderComp>();
	CylinderComponent2->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 0.0f), FVector(90, 0, 0), FVector(1, 1, 1)));
	CylinderComponent2->SetupAttachment(RootComponent);

	UCylinderComp* CylinderComponent3 = AddComponent<UCylinderComp>();
	CylinderComponent3->SetRelativeTransform(FTransform(FVector(0.0f, 0.0f, 0.0f), FVector(0, 90, 0), FVector(1, 1, 1)));
	CylinderComponent3->SetupAttachment(RootComponent);

    SetActorTransform(FTransform());
}

void ACylinder::BeginPlay()
{
    Super::BeginPlay();
}

void ACylinder::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
	FTransform tr = GetActorTransform();
	tr.Rotate(FVector(0.0f, 10.0f, 0.0f));
	SetActorTransform(tr);
}

const char* ACylinder::GetTypeName()
{
    return "Cylinder";
}
