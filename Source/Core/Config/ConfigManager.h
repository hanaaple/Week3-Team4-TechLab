#pragma once
#include "Core/Container/Map.h"
#include "Core/Container/String.h"
#include "Core/AbstractClass/Singleton.h"


class UConfigManager : public TSingleton<UConfigManager>
{
public:
	using Section = TMap<FString, FString>;
	using ConfigData = TMap<FString, Section>;

	/** ini 파일을 읽어들입니다. */
	bool LoadConfig(const FString& InConfigName);

	/** ini 파일을 저장합니다. */
	bool SaveConfig(const FString& InConfigName);

	/** Section과 Key를 이용하여 값을 가져옵니다. */
	FString GetValue(const FString& InSection, const FString& InKey);

	/** Section과 Key를 이용하여 Value을 설정합니다. */
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
	/** 문자열 앞뒤의 공백 제거 함수 */
	std::string trim(const std::string& InStr) const;

private:
	ConfigData Configs;
};

