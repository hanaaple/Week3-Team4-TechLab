#pragma once
#include "Slate/SSplitter.h"
#include "FViewportClient.h"

#include <d3d11.h>

class FViewport
{
public:
	FViewport(FViewportClient* InClient, FRect& InRect) :
		Client(InClient), Rect(InRect)
	{
		Viewport = { Rect.Left, Rect.Top, Rect.Right - Rect.Left,  Rect.Bottom - Rect.Top, 0.0f, 1.0f };
	}

	D3D11_VIEWPORT GetViewport() const { return Viewport; }
	FViewportClient* GetClient() const { return Client; }
	FRect GetRect() const { return Rect; }
	void SetRect(FRect InRect)
	{
		Rect = InRect;
		Viewport = { Rect.Left, Rect.Top, Rect.Right - Rect.Left,  Rect.Bottom - Rect.Top, 0.0f, 1.0f };
	}
private:
	D3D11_VIEWPORT Viewport;
	FViewportClient* Client;
	FRect Rect;
};