#pragma once

enum class EViewType : uint8
{
	Top,
	Bottom,
	Left,
	Right,
	Front,
	Back,
	Perspective
};

class FViewportClient
{
public:
	FViewportClient() { ViewType = EViewType::Top; }

	FViewportClient(EViewType InViewType)
	{
		ViewType = InViewType;
	}
	
	EViewType GetViewType() { return ViewType; }
private:
	EViewType ViewType;
};