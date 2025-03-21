#pragma once
#include "Actor.h"

class ASpotLight : public AActor
{
	DECLARE_CLASS(ASpotLight, AActor)

public:
	ASpotLight();
	virtual ~ASpotLight() = default;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual const FString GetTypeName() override;
};

