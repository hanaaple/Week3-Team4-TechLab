#include "LightComponentBase.h"

ULightComponentBase::ULightComponentBase()
	: bLightEnabled(true)
	, LightColor(1.0f, 1.0f, 1.0f, 1.0f)
	, Intensity(1.0f)
	, bCastShadows(true)
	, AttenuationRadius(100.0f)
{
}

ULightComponentBase::~ULightComponentBase()
{
}

void ULightComponentBase::BeginPlay()
{
	USceneComponent::BeginPlay();
}

void ULightComponentBase::Tick(float DeltaTime)
{
	USceneComponent::Tick(DeltaTime);
}

void ULightComponentBase::Render()
{

}

void ULightComponentBase::SetLightEnabled(bool bEnabled)
{
	bLightEnabled = bEnabled;
}

void ULightComponentBase::SetLightColor(const FVector4& InColor)
{
	LightColor = InColor;
}

void ULightComponentBase::SetIntensity(float InIntensity)
{
	Intensity = InIntensity;
}

void ULightComponentBase::SetCastShadows(bool bCast)
{
	bCastShadows = bCast;
}
