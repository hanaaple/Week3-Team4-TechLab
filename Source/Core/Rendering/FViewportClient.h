#pragma once

enum class EViewType : uint8
{
	Top,
	Bottom,
	Left,
	Right,
	Front,
	Back,
	Perspective,
	None
};

class FViewportClient
{
public:
	FViewportClient() { ViewType = EViewType::Top; }

	FViewportClient(EViewType InViewType)
	{
		ViewType = InViewType;
	}

private:
	EViewType ViewType;
};