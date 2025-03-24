#pragma once
#include "Core/Container/Array.h"
#include "Core/Container/String.h"
#include "ImGui/imgui.h"


class AActor;
class FViewport;

class UI
{
public:
    int currentItem = 0;
    int NumOfSpawn = 0;
	bool bIsInitialized = false;
    
    void Initialize(HWND hWnd, const class FDevice& Device, UINT ScreenWidth, UINT ScreenHeight);
    void Update();
    void Shutdown() const;

	void OnUpdateWindowSize(UINT InScreenWidth, UINT InScreenHeight);

public:// UIWindows
    void RenderControlPanel();
	void RenderMemoryUsage() const;
    void RenderPrimitiveSelection();
    void RenderCameraSettings() const;
    void RenderPropertyWindow() const;
    void RenderSceneManager();
	void RenderShowFlagsPanel() const;
	//void RenderViewModePanel() const;
	void RenderGridSettings() const;
	void RenderViewportSettings(FViewport* InViewport, int32 num);
private:
	// Mouse 전용
	ImVec2 ResizeToScreenByCurrentRatio(const ImVec2& vec2) const
	{
		return {vec2.x / CurRatio.x, vec2.y / CurRatio.y };
	}
	
    ImVec2 ResizeToScreen(const ImVec2& vec2) const
    {
		float ratio = GetMin();
		float preMin = GetPreMin();
    	return {vec2.x * PreRatio.x / CurRatio.x * ratio / preMin, vec2.y * PreRatio.y / CurRatio.y * ratio / preMin};
    }

    ImVec2 GetRatio() const
    {
    	return {ScreenSize.x / InitialScreenSize.x, ScreenSize.y / InitialScreenSize.y};
    }

	float GetMin() const
	{
		if (CurRatio.x < CurRatio.y)
		{
			return CurRatio.x;
		}
		else
		{
			return CurRatio.y;
		}
	}

	float GetPreMin() const
	{
		if (PreRatio.x < PreRatio.y)
		{
			return PreRatio.x;
		}
		else
		{
			return PreRatio.y;
		}
	}
	bool bWasWindowSizeUpdated = true;
	
    ImVec2 ScreenSize;
	ImVec2 InitialScreenSize;

	ImVec2 PreRatio;
	ImVec2 CurRatio;

	TArray<FString> UUIDNames;
	TArray<uint32> UUIDs;
	uint32 PrevSize = 0;
	AActor* CurActor = nullptr;
};
