﻿#pragma once
#include "UObject.h"
#include "Core/Math/Vector.h"
#include "Core/Container/Set.h"
#include "Core/Math/Transform.h"
#include "Object/ActorComponent/ActorComponent.h"

class USceneComponent : public UActorComponent
{
	using Super = UActorComponent;
public:
	USceneComponent() = default;
	virtual ~USceneComponent() = default;

public:	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


	const FTransform& GetComponentTransform() { return Transform; }

	void SetTransform(const FTransform& InTransform);
	bool CanEverTick() const { return bCanEverTick; }

public:
	void SetupAttachment(USceneComponent* InParent, bool bUpdateChildTransform = false);

protected:
	void UpdateChildrenTransform();

protected:
	USceneComponent* Parent = nullptr;
	TSet<USceneComponent*> Children;
	FTransform Transform;
	bool bCanEverTick = true;
};