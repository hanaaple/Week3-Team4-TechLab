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
		ViewMode = EViewModeIndex::VMI_Wireframe;
	}

	ELevelViewportType GetLevelViewportType() const { return LevelViewportType; }
	EViewModeIndex GetViewMode() const { return ViewMode; }

	void SetLevelViewportType(const ELevelViewportType InLevelViewportType) { LevelViewportType = InLevelViewportType; }
	void SetViewMode(const EViewModeIndex InViewMode) { ViewMode = InViewMode; }

	ACamera* GetPerspectiveCamera() const { return PerspectiveCamera; }
	ACamera* GetOrthographicCamera() const { return OrthographicCamera; }

private:
	ELevelViewportType LevelViewportType;
	EViewModeIndex ViewMode;
	ACamera* PerspectiveCamera;
	ACamera* OrthographicCamera;
};