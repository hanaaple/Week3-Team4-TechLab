#pragma once
#include "FViewMode.h"

enum class ELevelViewportType : uint32
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
		LevelViewportType = ELevelViewportType::Top;
		PerspectiveCamera = InPerspectiveCamera;
		OrthographicCamera = InOrthographicCamera;
		ViewMode = ERenderModeIndex::VMI_Wireframe;
	}

	ELevelViewportType GetViewType() const { return LevelViewportType; }
	ERenderModeIndex GetRenderType() const { return ViewMode; }

	void SetViewType(const ELevelViewportType InViewType) { LevelViewportType = InViewType; }
	void SetRenderType(const ERenderModeIndex InRenderType) { ViewMode = InRenderType; }

	ACamera* GetPerspectiveCamera() const { return PerspectiveCamera; }
	ACamera* GetOrthographicCamera() const { return OrthographicCamera; }

private:
	ELevelViewportType LevelViewportType;
	ERenderModeIndex ViewMode;
	ACamera* PerspectiveCamera;
	ACamera* OrthographicCamera;
};