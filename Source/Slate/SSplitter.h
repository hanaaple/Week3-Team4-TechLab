#pragma once

#include <memory>

#include "SWindow.h"

class SSplitter : public SWindow
{
protected:
	/** 두 개의 자식 창 */
	std::unique_ptr<SWindow> SideLT; // Left or Top
	std::unique_ptr<SWindow> SideRB; // Right or Bottom

public:
	SSplitter(const float Left, const float Top, const float Right, const float Bottom)
		: SWindow(Left, Top, Right, Bottom) {
	}
	SSplitter(const FRect& Rect)
		: SWindow(Rect.Left, Rect.Top, Rect.Right, Rect.Bottom) {
	}

	~SSplitter() override = default;

	void SetChildren(std::unique_ptr<SWindow> LT, std::unique_ptr<SWindow> RB) {
		SideLT = std::move(LT);
		SideRB = std::move(RB);
	}

	virtual bool IsHover(const FPoint& Coord) const override {
		if (SideLT && SideLT->IsHover(Coord)) {
			return true;
		}
		if (SideRB && SideRB->IsHover(Coord)) {
			return true;
		}
		return false;
	}

	SWindow* GetSideLT() { return SideLT.get(); }
	SWindow* GetSideRB() { return SideRB.get(); }
};

class SSplitterH : public SSplitter
{
public:
	SSplitterH(const float Left, const float Top, const float Right, const float Bottom)
		: SSplitter(Left, Top, Right, Bottom) {
	}
	SSplitterH(const FRect& Rect)
		: SSplitter(Rect) {
	}

	/** 수평으로 자식 창을 생성 및 설정 */
	void SplitHorizontally(float MidY) {
		SetChildren(
			std::make_unique<SWindow>(Rect.Left, Rect.Top, Rect.Right, MidY),   // 위쪽 영역
			std::make_unique<SWindow>(Rect.Left, MidY, Rect.Right, Rect.Bottom) // 아래쪽 영역
		);
	}
};

class SSplitterV : public SSplitter
{
public:
	SSplitterV(const float Left, const float Top, const float Right, const float Bottom)
		: SSplitter(Left, Top, Right, Bottom) {
	}
	SSplitterV(const FRect& Rect)
		: SSplitter(Rect) {
	}

	void SplitVertically(float MidX) {
		SetChildren(
			std::make_unique<SWindow>(Rect.Left, Rect.Top, MidX, Rect.Bottom),   // 왼쪽 영역
			std::make_unique<SWindow>(MidX, Rect.Top, Rect.Right, Rect.Bottom)  // 오른쪽 영역
		);
	}
};