#pragma once
#include <vector>
#include "ImGui/imgui.h"
#include <assert.h>


#define MsgBoxAssert(Text) \
std::string Value = Text; \
MessageBoxA(nullptr, Value.c_str(), "Error", MB_OK); assert(false);


#define UE_LOG(format, ...) Debug::Log(format, ##__VA_ARGS__)


class FString;

class Debug
{
    static std::vector<FString> items; // 출력 로그

public:
    static void ShowConsole(bool bWasWindowSizeUpdated, ImVec2 PreRatio, ImVec2 CurRatio);
    static void ProcessCommand(const FString& command, std::vector<FString>& log);
    static void Log(const char* format, ...);
    static ImVec2 ResizeToScreen(const ImVec2& vec2, ImVec2 PreRatio, ImVec2 CurRatio);
};
