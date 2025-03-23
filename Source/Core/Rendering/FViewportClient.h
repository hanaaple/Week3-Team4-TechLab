#pragma once
#include "FViewMode.h"
class FViewportClient
{
public:
	FViewportClient(ACamera* InPerspectiveCamera, ACamera* InOrthographicCamera)
		: ViewType(TEXT("Top")), PerspectiveCamera(InPerspectiveCamera), OrthographicCamera(InOrthographicCamera)
	{
	}

	FViewportClient(const FString InViewType, ACamera* InPerspectiveCamera, ACamera* InOrthographicCamera)
		: ViewType(InViewType), PerspectiveCamera(InPerspectiveCamera), OrthographicCamera(InOrthographicCamera)
	{
	}

	FString GetViewType() const { return ViewType; }
	EViewModeIndex GetRenderType() const { return RenderType; }

	void SetViewType(const FString InViewType) { ViewType = InViewType; }
	void SetRenderType(const EViewModeIndex InRenderType) { RenderType = InRenderType; }

	ACamera* GetPerspectiveCamera() const { return PerspectiveCamera; }
	ACamera* GetOrthographicCamera() const { return OrthographicCamera; }

	bool GetSelected() const { return SelectedFlag; }
	void SetSelected(bool InSelectedFlag) { SelectedFlag = InSelectedFlag; }
private:
	FString ViewType;
	EViewModeIndex RenderType;
	ACamera* PerspectiveCamera;
	ACamera* OrthographicCamera;
	bool SelectedFlag;
};