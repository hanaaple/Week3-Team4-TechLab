#pragma once
#include "Slate/SSplitter.h"
#include "FViewportClient.h"

#include <d3d11.h>

class FViewport
{
public:
	FViewport(FViewportClient* InClient, FRect Rect) :
		Client(InClient)
	{
		Viewport = { Rect.Left, Rect.Top, Rect.Right - Rect.Left,  Rect.Bottom - Rect.Top, 0.0f, 1.0f };
	}

	D3D11_VIEWPORT GetViewport() const { return Viewport; }
	FViewportClient* GetClient() const { return Client; }

	bool IsMouseInside(float mouseX, float mouseY) const
	{
		float left = Viewport.TopLeftX;
		float top = Viewport.TopLeftY;
		float right = left + Viewport.Width;
		float bottom = top + Viewport.Height;

		return (mouseX >= left && mouseX < right &&
			mouseY >= top && mouseY < bottom);
	}
private:
	D3D11_VIEWPORT Viewport;
	FViewportClient* Client;
};