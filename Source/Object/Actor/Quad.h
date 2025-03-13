#pragma once
#include "Actor.h"
class AQuad : public AActor
{
	using Super = AActor;
public:
	AQuad();
	virtual ~AQuad() = default;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual const char* GetTypeName() override;
};

