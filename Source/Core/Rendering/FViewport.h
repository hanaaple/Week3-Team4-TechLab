#pragma once
#include "Slate/SSplitter.h"
#include "FViewportClient.h"

#include <d3d11.h>

class FViewport
{
public:
	FViewport(FViewportClient* InClient, FRect Rect, ACamera* InCamera) : Client(InClient), Camera(InCamera)
	{
		Viewport = { Rect.Left, Rect.Top, Rect.Right - Rect.Left,  Rect.Bottom - Rect.Top, 0.0f, 1.0f };
	}
	D3D11_VIEWPORT GetViewport() const { return Viewport; }
	FViewportClient* GetClient() const { return Client; }
	ACamera* GetCamera() const { return Camera; }
private:
	D3D11_VIEWPORT Viewport;
	FViewportClient* Client;
	ACamera* Camera;
};