#pragma once
#include "Actor.h"

class AQuad : public AActor
{
	DECLARE_CLASS(AQuad, AActor)

public:
	AQuad();
	virtual ~AQuad() = default;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual const char* GetTypeName() override;
};

