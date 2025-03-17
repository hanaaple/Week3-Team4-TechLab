#include "BlendState.h"

#include <d3dcompiler.h>
#include "Core/Rendering/FDevice.h"
#include "Debug/DebugConsole.h"


FBlendState::FBlendState()
{
}

FBlendState::~FBlendState()
{
	if (nullptr != State)
	{
		State->Release();
		State = nullptr;
	}
}

void FBlendState::Setting()
{
	if (nullptr == State)
	{
		UE_LOG("Error: BlendState Setting Failed");
	}

	FDevice::Get().GetDeviceContext()->OMSetBlendState(State, nullptr, Mask);
}

void FBlendState::ResCreate(const D3D11_BLEND_DESC& _Desc)
{
	Desc = _Desc;
	if (S_OK != FDevice::Get().GetDevice()->CreateBlendState(&_Desc, &State))
	{
		UE_LOG("Error: BlendState Setting Failed");
	}
}
