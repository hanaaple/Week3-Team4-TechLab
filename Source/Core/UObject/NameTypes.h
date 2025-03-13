#pragma once
#include "Core/HAL/PlatformType.h"

class FString;

class FName
{
	// uint32 DisplayIndex;    // 원본 문자열의 Hash
	uint32 ComparisonIndex; // 비교시 사용되는 Hash

public:
	FName() : ComparisonIndex(0) {}
	FName(const WIDECHAR* Name);
	FName(const ANSICHAR* Name);
	FName(const FString& Name);

	FString ToString() const;
	// bool Equals(const FName& Other) const;

	bool operator==(const FName& Other) const;
};
