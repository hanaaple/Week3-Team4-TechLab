#pragma once
#include "Actor.h"


class AQuad : public AActor
{
	DECLARE_CLASS(AQuad, AActor)

public:
	AQuad();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual const FString GetTypeName() override;
};

