#pragma once
#include <cstdint>
#include <unordered_map>
#include <string>
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

	// 콘솔 명령어로 플래그를 설정할 수 있도록 함수 추가
	bool SetFlagByName(const std::string& FlagName, bool bEnabled);

	// 플래그 이름으로 인덱스 찾기
	static int32 FindIndexByName(const std::string& FlagName);

private:
	FEngineShowFlags() { Initialize(); }
	~FEngineShowFlags() = default;

	uint64 ShowFlagBits;
	std::unordered_map<std::string, EEngineShowFlags> NameToFlagMap;
};
