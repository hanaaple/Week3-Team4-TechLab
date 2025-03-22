#pragma once

#include "Core/HAL/PlatformType.h"

// FVector2f
struct FPoint
{
	float X, Y;
};

struct FRect
{
	float Left;
	float Top;
	float Right;
	float Bottom;

	// Check if the Point is in the Rect
	bool Contains(const FPoint& point) const
	{
		return	point.X >= Left && point.X <= Right &&
				point.Y >= Top && point.Y <= Bottom;
	}
};

class SWindow
{
protected:
	FRect Rect;

public:
	SWindow(const float Left, const float Top, const float Right, const float Bottom)
		: Rect{ .Left= Left, .Top= Top, .Right= Right, .Bottom= Bottom}
	{}

	virtual ~SWindow() = default;

	virtual bool IsHover(const FPoint& Coord) const {
		return Rect.Contains(Coord);
	}

	const FRect& GetRect() const { return Rect; }

	void SetRect(const float Left, const float Top, const float Right, const float Bottom) {
		Rect = { .Left= Left, .Top= Top, .Right= Right, .Bottom= Bottom};
	}

	void SetRect(const FRect& NewRect) {
		Rect = NewRect;
	}
};