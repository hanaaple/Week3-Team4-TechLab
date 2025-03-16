#pragma once
#include "Core/Container/Map.h"
#include "Core/Container/String.h"
#include "Core/HAL/PlatformType.h"

enum class EEngineShowFlags : uint64
{
	SF_Primitives = 1 << 0,
	SF_BillboardText = 1 << 1,
};

class FEngineShowFlags
{
public:
	static FEngineShowFlags& Get()
	{
		static FEngineShowFlags Instance;
		return Instance;
	}

	void Initialize();

	bool GetSingleFlag(EEngineShowFlags Flag) const;
	void SetSingleFlag(EEngineShowFlags Flag, bool bEnabled);
	void ToggleSingleFlag(EEngineShowFlags Flag);

	bool SetFlagByName(const FString& FlagName, bool bEnabled);

	static int32 FindIndexByName(const FString& FlagName);
private:
	FEngineShowFlags() { Initialize(); }
	~FEngineShowFlags() = default;

	uint64 ShowFlagBits;
	TMap<FString, EEngineShowFlags> NameToFlagMap;
};
