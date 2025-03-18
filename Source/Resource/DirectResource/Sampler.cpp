#include "Sampler.h"

#include <d3dcompiler.h>
#include "Core/Rendering/FDevice.h"

#include "Debug/DebugConsole.h"
#include "DirectXTK/DDSTextureLoader.h"

FSampler::FSampler()
{
}

FSampler::~FSampler()
{
	if (nullptr != State)
	{
		State->Release();
		State = nullptr;
	}
}

void FSampler::VSSetting(UINT _Slot)
{
	FDevice::Get().GetDeviceContext()->VSSetSamplers(_Slot, 1, &State);
}

void FSampler::PSSetting(UINT _Slot)
{
	FDevice::Get().GetDeviceContext()->PSSetSamplers(_Slot, 1, &State);
}



void FSampler::ResCreate(const D3D11_SAMPLER_DESC& _Desc)
{
	Desc = _Desc;
	if (S_OK != FDevice::Get().GetDevice()->CreateSamplerState(&_Desc, &State))
	{
		MsgBoxAssert("샘플러 세팅을 만드는데 실패했습니다.");
	}
}
