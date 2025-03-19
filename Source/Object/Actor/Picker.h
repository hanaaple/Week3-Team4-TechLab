#pragma once
#include "Actor.h"
#include "Core/Interfaces/GizmoInterface.h"


class APicker : public AActor, public IGizmoInterface
{
	DECLARE_CLASS(APicker, AActor)

public:
    APicker() = default;

	//~ Begin IGizmoInterface
	virtual bool IsGizmo() override { return true; }
	//~ End IGizmoInterface

    static FVector4 EncodeUUID(unsigned int UUID);
    static int DecodeUUID(FVector4 color);

	virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void LateTick(float DeltaTime) override;
    virtual const char* GetTypeName() override;

	void TestFunc();
};
