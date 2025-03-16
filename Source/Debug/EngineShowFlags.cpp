#include "EngineShowFlags.h"

void FEngineShowFlags::Initialize()
{
	// 기본적으로 모든 플래그 활성화
	ShowFlagBits = UINT64_MAX;

	// 이름과 플래그 매핑 초기화
	NameToFlagMap["Primitives"] = EEngineShowFlags::SF_Primitives;
	NameToFlagMap["BillboardText"] = EEngineShowFlags::SF_BillboardText;
}

bool FEngineShowFlags::GetSingleFlag(EEngineShowFlags Flag) const
{
	return (ShowFlagBits & static_cast<uint64>(Flag)) != 0;
}

void FEngineShowFlags::SetSingleFlag(EEngineShowFlags Flag, bool bEnabled)
{
	if (bEnabled)
		ShowFlagBits |= static_cast<uint64>(Flag);
	else
		ShowFlagBits &= ~static_cast<uint64>(Flag);
}

void FEngineShowFlags::ToggleSingleFlag(EEngineShowFlags Flag)
{
	ShowFlagBits ^= static_cast<uint64>(Flag);
}

bool FEngineShowFlags::SetFlagByName(const FString& FlagName, bool bEnabled)
{
	auto res = NameToFlagMap.Find(FlagName);
	if (res)
	{
		SetSingleFlag(*res, bEnabled);
		return true;
	}
	return false;
}

int32 FEngineShowFlags::FindIndexByName(const FString& FlagName)
{
	auto& Instance = Get();
	auto res = Instance.NameToFlagMap.Find(FlagName);
	if (res)
	{
		return static_cast<int32>(*res);
	}
	return -1;
}
