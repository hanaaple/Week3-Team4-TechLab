﻿#pragma once
#include "Object/Actor/Actor.h"
#include "Object/PrimitiveComponent/UPrimitiveComponent.h"

class AAxis : public AActor
{
	using Super = AActor;
public:
	AAxis();
	virtual ~AAxis() = default;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};

