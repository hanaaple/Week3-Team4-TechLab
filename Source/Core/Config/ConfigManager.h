#pragma once
#include "Core/Container/String.h"
#include "Core/Container/Map.h"
#include "Core/AbstractClass/Singleton.h"

using Section = TMap<FString, FString>;
using ConfigData = TMap<FString, Section>;


class UConfigManager : public TSingleton<UConfigManager>
{
public:
	/// <summary>
	/// ini 파일을 읽어들입니다.
	/// </summary>
	bool LoadConfig(const FString& InConfigName);

	/// <summary>
	/// ini 파일을 저장합니다.
	/// </summary>
	bool SaveConfig(const FString& InConfigName);

	/// <summary>
	/// Section과 Key를 이용하여 값을 가져옵니다.
	/// </summary>
	FString GetValue(const FString& InSection, const FString& InKey);

	/// <summary>
	/// Section과 Key를 이용하여 Value을 설정합니다.
	/// </summary>
	void SetValue(const FString& InSection, const FString& InKey, const FString& InValue);

	uint32 GetSectionCount() const { return Configs.Num(); }

	uint32 GetKeyCount(const FString& InSection) const
	{
		if (const auto Section = Configs.Find(InSection))
		{
			return Section->Num();
		}
		return 0;
	}


private:
	/// <summary>
	/// 문자열 앞뒤의 공백 제거 함수
	/// </summary>
	std::string trim(const std::string& InStr) const;

private:
	ConfigData Configs;
};

