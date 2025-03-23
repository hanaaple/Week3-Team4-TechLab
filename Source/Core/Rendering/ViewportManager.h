#pragma once
#include "FViewport.h"
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
	void UnregisterViewport(FViewport* InViewport)
	{
		Viewports.Remove(InViewport);
		if (ActiveViewport == InViewport)
		{
			ActiveViewport = nullptr;
		}
	}

	// 특정 뷰포트를 활성 뷰포트로 설정합니다.
	void SetActiveViewport(FViewport* NewActiveViewport)
	{
		ActiveViewport = NewActiveViewport;
		for (FViewport* vp : Viewports)
		{
			if (vp->GetClient())
			{
				vp->GetClient()->SetSelected(vp == ActiveViewport);
			}
		}
	}

	// 현재 활성 뷰포트를 반환합니다.
	FViewport* GetActiveViewport() const { return ActiveViewport; }

	// 등록된 모든 뷰포트의 배열을 반환합니다.
	const TArray<FViewport*>& GetViewports() const { return Viewports; }

private:
	TArray<FViewport*> Viewports;
	FViewport* ActiveViewport = nullptr;
};