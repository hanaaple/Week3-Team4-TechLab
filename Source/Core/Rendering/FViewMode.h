#pragma once
#define _TCHAR_DEFINED
#include <d3d11.h>
#include "Core/HAL/PlatformType.h"
#include "Core/Container/Map.h"


enum class EViewModeIndex : uint32
{
	// Lit mode는 추후 업데이트
	//VMI_Lit, 
	//VMI_Unlit,
	VMI_Solid,
	VMI_Wireframe,
};

class FViewMode
{
public:
	static FViewMode& Get()
	{
		static FViewMode Instance;
		return Instance;
	}

	void Initialize(ID3D11Device* Device);
	void SetViewMode(EViewModeIndex ViewMode);
	void ApplyViewMode(ID3D11DeviceContext* DeviceContext);

	EViewModeIndex GetViewMode() const { return CurrentViewMode; }
private:
	FViewMode() : CurrentViewMode(EViewModeIndex::VMI_Solid), Device(nullptr) {}
	~FViewMode();

	void CreateRasterizerStates();

	EViewModeIndex CurrentViewMode;
	ID3D11Device* Device;

	TMap<EViewModeIndex, ID3D11RasterizerState*> RasterizerStates;
};
