#pragma once

#include <memory>

#include "SWindow.h"

class SSplitter : public SWindow
{
protected:
	/** 두 개의 자식 창 */
	SWindow* SideLT; // Left or Top
	SWindow* SideRB; // Right or Bottom

	FRect Border;
	float BorderSize = 5.f;
	bool bIsBorderDragging = false;

public:
	SSplitter(const float Left, const float Top, const float Right, const float Bottom)
		: SWindow(Left, Top, Right, Bottom) {}
	SSplitter(const FRect& Rect)
		: SWindow(Rect.Left, Rect.Top, Rect.Right, Rect.Bottom) {}

	~SSplitter() override = default;

	void SetChildren(SWindow* LT, SWindow* RB) {
		SideLT = LT;
		SideRB = RB;
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

	SWindow* GetSideLT() { return SideLT; }
	SWindow* GetSideRB() { return SideRB; }

	bool IsBorderHover(const FPoint& Coord) const {
		return Border.Contains(Coord);
	}

	bool GetIsBorderDragging() const { return bIsBorderDragging; }
	void SetIsBorderDragging(bool bDragging) { bIsBorderDragging = bDragging; }
};

class SSplitterH : public SSplitter
{
public:
	SSplitterH(const float Left, const float Top, const float Right, const float Bottom)
		: SSplitter(Left, Top, Right, Bottom) {}
	SSplitterH(const FRect& Rect)
		: SSplitter(Rect) {}

	/** 수평으로 자식 창을 생성 및 설정 */
	void SplitHorizontally(float MidY) {
		SetChildren(
			new SWindow(Rect.Left, Rect.Top, Rect.Right, MidY),	// 위쪽 영역
			new SWindow(Rect.Left, MidY, Rect.Right, Rect.Bottom)	// 아래쪽 영역
		);
		Border = { .Left= Rect.Left, .Top= MidY - BorderSize/2, .Right= Rect.Right, .Bottom= MidY + BorderSize/2 };
	}

	void MoveBorder(float DeltaY)
	{
		SideLT->SetRect(SideLT->GetRect().Left, SideLT->GetRect().Top, SideLT->GetRect().Right, SideLT->GetRect().Bottom + DeltaY);
		SideRB->SetRect(SideRB->GetRect().Left, SideRB->GetRect().Top + DeltaY, SideRB->GetRect().Right, SideRB->GetRect().Bottom);
		UpdateBorder(DeltaY);
	}

	void MoveParentBorder(float DeltaX, bool bIsLT)
	{
		Rect.Left = bIsLT ? Rect.Left : Rect.Left + DeltaX;
		Rect.Right = bIsLT ? Rect.Right + DeltaX : Rect.Right;
		SideLT->SetRect(bIsLT ? SideLT->GetRect().Left : SideLT->GetRect().Left + DeltaX, SideLT->GetRect().Top, bIsLT ? SideLT->GetRect().Right + DeltaX : SideLT->GetRect().Right, SideLT->GetRect().Bottom);
		SideRB->SetRect(bIsLT ? SideRB->GetRect().Left : SideRB->GetRect().Left + DeltaX, SideRB->GetRect().Top, bIsLT ? SideRB->GetRect().Right + DeltaX : SideRB->GetRect().Right, SideRB->GetRect().Bottom);
		Border.Left = bIsLT ? Rect.Left : Rect.Left + DeltaX;
		Border.Right = bIsLT ? Rect.Right + DeltaX : Rect.Right;
	}

	void UpdateBorder(float DeltaY) {
		Border.Top += DeltaY;
		Border.Bottom += DeltaY;
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
			new SWindow(Rect.Left, Rect.Top, MidX, Rect.Bottom),	// 왼쪽 영역
			new SWindow(MidX, Rect.Top, Rect.Right, Rect.Bottom)	// 오른쪽 영역
		);
		Border = { .Left = MidX - BorderSize / 2, .Top = Rect.Top, .Right = MidX + BorderSize / 2, .Bottom = Rect.Bottom };
	}

	void MoveBorder(float DeltaX)
	{
		SideLT->SetRect(SideLT->GetRect().Left, SideLT->GetRect().Top, SideLT->GetRect().Right + DeltaX, SideLT->GetRect().Bottom);
		SideRB->SetRect(SideRB->GetRect().Left + DeltaX, SideRB->GetRect().Top, SideRB->GetRect().Right, SideRB->GetRect().Bottom);
		UpdateBorder(DeltaX);
	}

	void MoveParentBorder(float DeltaY, bool bIsLT)
	{
		Rect.Top = bIsLT ? Rect.Top : Rect.Top + DeltaY;
		Rect.Bottom = bIsLT ? Rect.Bottom + DeltaY : Rect.Bottom;
		SideLT->SetRect(SideLT->GetRect().Left, bIsLT ? SideLT->GetRect().Top : SideLT->GetRect().Top + DeltaY, SideLT->GetRect().Right, bIsLT ? SideLT->GetRect().Bottom + DeltaY : SideLT->GetRect().Bottom);
		SideRB->SetRect(SideRB->GetRect().Left, bIsLT ? SideRB->GetRect().Top : SideRB->GetRect().Top + DeltaY, SideRB->GetRect().Right, bIsLT ? SideRB->GetRect().Bottom + DeltaY : SideRB->GetRect().Bottom);
		Border.Top = bIsLT ? Rect.Top : Rect.Top + DeltaY;
		Border.Bottom = bIsLT ? Rect.Bottom + DeltaY : Rect.Bottom;
	}

	void UpdateBorder(float DeltaX) {
		Border.Left += DeltaX;
		Border.Right += DeltaX;
	}
};