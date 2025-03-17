#pragma once
#include <string>
#include "Core/Container/Map.h"
#include "Core/AbstractClass/Singleton.h"

using Section = TMap<std::string, std::string>;
using ConfigData = TMap<std::string, Section>;


class UConfigManager : public TSingleton<UConfigManager>
{
public:
	/** ini 파일을 읽어들입니다. */
	bool LoadConfig(const std::string& InConfigName);

	/** ini 파일을 저장합니다. */
	bool SaveConfig(const std::string& InConfigName);

	/** Section과 Key를 이용하여 값을 가져옵니다. */
	std::string GetValue(const std::string& InSection, const std::string& InKey);

	/** Section과 Key를 이용하여 Value을 설정합니다. */
	void SetValue(const std::string& InSection, const std::string& InKey, const std::string& InValue);

private:
	/** 문자열 앞뒤의 공백 제거 함수 */
	std::string trim(const std::string& InStr) const;

private:
	ConfigData Configs;
};

