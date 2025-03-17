#include "Cylinder.h"

#include <Object/PrimitiveComponent/UPrimitiveComponent.h>
#include "Core/Input/PlayerInput.h"

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
	
	FTransform NewTransform = GetActorTransform();
	FVector NewPosition = NewTransform.GetPosition();
	FVector NewRotation = NewTransform.GetRotation().GetEuler();
	FVector NewScale = NewTransform.GetScale();

	if (APlayerInput::Get().GetKeyDown(EKeyCode::Up))
	{
		NewPosition += FVector(0, 0, 1);
	}
	if (APlayerInput::Get().GetKeyDown(EKeyCode::Down))
	{
		NewPosition += FVector(0, 0, -1);
	}
	if (APlayerInput::Get().GetKeyDown(EKeyCode::Left))
	{
		NewPosition += FVector(0, 1, 0);
	}
	if (APlayerInput::Get().GetKeyDown(EKeyCode::Right))
	{
		NewPosition += FVector(0, -1, 0);
	}

	if (APlayerInput::Get().GetKeyDown(EKeyCode::J))
	{
		NewScale += FVector(0.1, 0, 0);
	}
	if (APlayerInput::Get().GetKeyDown(EKeyCode::K))
	{
		NewScale += FVector(0, 0.1, 0);
	}
	if (APlayerInput::Get().GetKeyDown(EKeyCode::L))
	{
		NewScale += FVector(0, 0, 0.1);
	}


	if (APlayerInput::Get().GetKeyPress(EKeyCode::Z))
	{
		NewRotation += FVector(1, 0, 0);
	}
	if (APlayerInput::Get().GetKeyPress(EKeyCode::X))
	{
		NewRotation += FVector(0, 1, 0);
	}
	if (APlayerInput::Get().GetKeyPress(EKeyCode::C))
	{
		NewRotation += FVector(0, 0, 1);
	}

	NewTransform.SetPosition(NewPosition);
	NewTransform.SetRotation(FQuat(NewRotation));
	NewTransform.SetScale(NewScale);
	SetActorTransform(NewTransform);

}

const char* ACylinder::GetTypeName()
{
    return "Cylinder";
}
