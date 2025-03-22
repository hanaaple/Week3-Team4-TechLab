#pragma once
#include "Actor.h"


class AStaticMeshObj : public AActor
{
	DECLARE_CLASS(AStaticMeshObj, AActor)
public:
	AStaticMeshObj();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual const FString GetTypeName() override;
};

