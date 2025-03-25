#pragma once
#include "FViewport.h"
#include "FViewportClient.h"
#include "Core/Container/Array.h"
#include "Core/Math/Vector.h"

class FViewportManager
{
public:
	FViewportManager() {};

	// 뷰포트를 등록합니다.
	void RegisterViewport(FViewport* InViewport)
	{
		Viewports.Add(InViewport);
	}

	// 뷰포트를 제거합니다.
	void UnregisterViewport()
	{
		for (FViewport* vp : Viewports)
		{
			delete vp;
		}
		Viewports.Empty();
	}

	void SetActiveViewport(FViewport* InActiveViewport) { ActiveViewport = InActiveViewport; }
	FViewport* GetActiveViewport() const { return ActiveViewport; }

	void SetFullScreenViewport(FViewport* InFullScreenViewport) { FullScreenViewport = InFullScreenViewport; }

	FViewport* GetFullScreenViewport() const { return FullScreenViewport; }
	
	// 등록된 모든 뷰포트의 배열을 반환합니다.
	const TArray<FViewport*>& GetViewports() const { return Viewports; }

private:
	TArray<FViewport*> Viewports;
	FViewport* ActiveViewport = nullptr;
	FViewport* FullScreenViewport = nullptr;
	bool blsFullscreen = true;
};