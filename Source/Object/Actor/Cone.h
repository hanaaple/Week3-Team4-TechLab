#pragma once
#include "Actor.h"


class ACone : public AActor
{
	DECLARE_CLASS(ACone, AActor)

public:
    ACone();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual const char* GetTypeName() override;
};

