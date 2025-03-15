#pragma once
#include "Core/UObject/ObjectMacros.h"
#include "Object/Actor/Actor.h"


class AArrow : public AActor
{
	DECLARE_CLASS(AArrow, AActor)

public:
	AArrow();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual const char* GetTypeName() override;
};

