#pragma once
#include "Core/EngineTypes.h"
#include "Core/UObject/Object.h"

struct FVector;

class UActorComponent : public UObject
{
public:
	UActorComponent() = default;

	virtual void BeginPlay();
	virtual void Tick(float DeltaTime);
	virtual void EndPlay(const EEndPlayReason::Type Reason);

	bool CanEverTick() const { return bCanEverTick; }

	virtual class AActor* GetOwner() const;
	virtual void SetOwner(class AActor* InOwner) { Owner = InOwner; }

	FVector GetActorPosition() const;

protected:
	bool bCanEverTick = true;
	class AActor* Owner = nullptr;
};

