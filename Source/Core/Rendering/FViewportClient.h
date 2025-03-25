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
		EngineShowFlags = UINT64_MAX;;
	}

	ELevelViewportType GetLevelViewportType() const { return LevelViewportType; }
	EViewModeIndex GetViewMode() const { return ViewMode; }
	uint64 GetEngineShowFlags() const { return EngineShowFlags; }

	void SetLevelViewportType(const ELevelViewportType InLevelViewportType) { LevelViewportType = InLevelViewportType; }
	void SetViewMode(const EViewModeIndex InViewMode) { ViewMode = InViewMode; }
	void SetEngineShowFlags(const uint64 InEngineShowFlags) { EngineShowFlags = InEngineShowFlags; }
	void ToggleEngineShowFlag(uint64 Flag)
	{
		EngineShowFlags ^= Flag;
	}

	ACamera* GetPerspectiveCamera() const { return PerspectiveCamera; }
	ACamera* GetOrthographicCamera() const { return OrthographicCamera; }

private:
	ELevelViewportType LevelViewportType;
	EViewModeIndex ViewMode;
	ACamera* PerspectiveCamera;
	ACamera* OrthographicCamera;
	uint64 EngineShowFlags;
};