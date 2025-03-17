#include "DepthStencilState.h"
#include <d3dcompiler.h>
#include "Core/Rendering/FDevice.h"
#include "Debug/DebugConsole.h"

FDepthStencilState::FDepthStencilState()
{
}

FDepthStencilState::~FDepthStencilState()
{
	if (nullptr != State)
	{
		State->Release();
		State = nullptr;
	}
}

void FDepthStencilState::Setting()
{
	if (nullptr == State)
	{
		UE_LOG("Error: FDepthStencilState Setting Failed");
	}

	FDevice::Get().GetDeviceContext()->OMSetDepthStencilState(State, 0);
}

void FDepthStencilState::ResCreate(const D3D11_DEPTH_STENCIL_DESC& _Desc)
{
	Desc = _Desc;
	if (S_OK != FDevice::Get().GetDevice()->CreateDepthStencilState(&Desc, &State))
	{
		UE_LOG("Error: FDepthStencilState Setting Failed");
	}
}
