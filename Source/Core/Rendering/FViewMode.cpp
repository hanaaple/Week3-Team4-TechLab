#include "FViewMode.h"
#include "Core/Rendering/FDevice.h" 

void FViewMode::Initialize(ID3D11Device* InDevice)
{
	Device = InDevice;
	CreateRasterizerStates();
	CurrentViewMode = EViewModeIndex::VMI_Default;
}

FViewMode::~FViewMode()
{
	// 래스터라이저 상태 해제
	for (auto pair : RasterizerStates)
	{
		if (pair.Value)
		{
			pair.Value->Release();
			pair.Value = nullptr;
		}
	}
	RasterizerStates.Empty();
}

void FViewMode::CreateRasterizerStates()
{
	if (!Device)
		return;

	// Solid 모드용 래스터라이저 상태
	D3D11_RASTERIZER_DESC solidDesc;
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidDesc.FillMode = D3D11_FILL_SOLID;
	solidDesc.CullMode = D3D11_CULL_BACK;
	solidDesc.FrontCounterClockwise = FALSE;
	solidDesc.DepthClipEnable = TRUE;
	solidDesc.MultisampleEnable = TRUE;

	// Wireframe 모드용 래스터라이저 상태
	D3D11_RASTERIZER_DESC wireframeDesc = solidDesc;
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;

	Device->CreateRasterizerState(&solidDesc, &RasterizerStates[EViewModeIndex::VMI_Solid]);
	Device->CreateRasterizerState(&wireframeDesc, &RasterizerStates[EViewModeIndex::VMI_Wireframe]);
}

void FViewMode::SetViewMode(EViewModeIndex ViewMode)
{
	CurrentViewMode = ViewMode;
}

void FViewMode::ApplyViewMode()
{
	FDevice::Get().GetDeviceContext()->RSSetState(RasterizerStates[CurrentViewMode]);
	
	// auto res = RasterizerStates.Find(CurrentViewMode);
	// if (res)
	// {
	// 	DeviceContext->RSSetState(*res);
	// }
}
