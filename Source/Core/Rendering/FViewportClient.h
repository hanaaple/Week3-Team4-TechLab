#pragma once
#include "FViewMode.h"

enum class EViewModeIndex : uint32
{
	Perspective,
	Top,
	Bottom,
	Left,
	Right,
	Front,
	Back
};

class FViewportClient
{
public:
	FViewportClient(ACamera* InPerspectiveCamera, ACamera* InOrthographicCamera)
	{
		ViewType = EViewModeIndex::Top;
		PerspectiveCamera = InPerspectiveCamera;
		OrthographicCamera = InOrthographicCamera;
		RenderType = ERenderModeIndex::VMI_Wireframe;
	}

	FViewportClient(const EViewModeIndex InViewType, ACamera* InPerspectiveCamera, ACamera* InOrthographicCamera)
		: ViewType(InViewType), PerspectiveCamera(InPerspectiveCamera), OrthographicCamera(InOrthographicCamera)
	{
		ViewType = InViewType;
		PerspectiveCamera = InPerspectiveCamera;
		OrthographicCamera = InOrthographicCamera;
		RenderType = ERenderModeIndex::VMI_Wireframe;
	}

	EViewModeIndex GetViewType() const { return ViewType; }
	ERenderModeIndex GetRenderType() const { return RenderType; }

	void SetViewType(const EViewModeIndex InViewType) { ViewType = InViewType; }
	void SetRenderType(const ERenderModeIndex InRenderType) { RenderType = InRenderType; }

	ACamera* GetPerspectiveCamera() const { return PerspectiveCamera; }
	ACamera* GetOrthographicCamera() const { return OrthographicCamera; }

private:
	EViewModeIndex ViewType;
	ERenderModeIndex RenderType;
	ACamera* PerspectiveCamera;
	ACamera* OrthographicCamera;
};