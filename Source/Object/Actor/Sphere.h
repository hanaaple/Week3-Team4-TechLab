#pragma once
#include "Object/Actor/Actor.h"


class ASphere : public AActor
{
	DECLARE_CLASS(ASphere, AActor)

public:
	ASphere();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual const FString GetTypeName() override;
};

