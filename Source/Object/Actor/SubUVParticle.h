#pragma once
#include "Actor.h"

class ASubUVParticle : public AActor
{
	DECLARE_CLASS(ASubUVParticle, AActor)

public:
	ASubUVParticle();
	virtual ~ASubUVParticle() = default;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};

