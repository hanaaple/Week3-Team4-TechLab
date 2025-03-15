#pragma once
#include <string>
#include "Core/Container/Map.h"
#include "Core/AbstractClass/Singleton.h"

using Section = TMap<std::string, std::string>;
using ConfigData = TMap<std::string, Section>;


class UConfigManager : public TSingleton<UConfigManager>
{
public:
	/// <summary>
	/// ini 파일을 읽어들입니다.
	/// </summary>
	bool LoadConfig(const std::string& InConfigName);

	/// <summary>
	/// ini 파일을 저장합니다.
	/// </summary>
	bool SaveConfig(const std::string& InConfigName);

	/// <summary>
	/// Section과 Key를 이용하여 값을 가져옵니다.
	/// </summary>
	std::string GetValue(const std::string& InSection, const std::string& InKey);

	/// <summary>
	/// Section과 Key를 이용하여 Value을 설정합니다.
	/// </summary>
	void SetValue(const std::string& InSection, const std::string& InKey, const std::string& InValue);

private:
	/// <summary>
	/// 문자열 앞뒤의 공백 제거 함수
	/// </summary>
	std::string trim(const std::string& InStr) const;

private:
	ConfigData Configs;
};

