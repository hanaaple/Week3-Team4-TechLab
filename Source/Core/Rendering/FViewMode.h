#pragma once
#define _TCHAR_DEFINED
#include <d3d11.h>
#include "Core/HAL/PlatformType.h"
#include "Core/Container/Map.h"


enum class ERenderModeIndex : uint32
{
	// Lit mode는 추후 업데이트
	//VMI_Lit, 
	//VMI_Unlit,
	VMI_Default, // 전역 레스트라이저를 적용하지 않음
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

	void Initialize(ID3D11Device* InDevice);
	void SetViewMode(ERenderModeIndex ViewMode);
	void ApplyViewMode();

	ERenderModeIndex GetViewMode() const { return CurrentViewMode; }
private:
	FViewMode() : CurrentViewMode(ERenderModeIndex::VMI_Solid), Device(nullptr) {}
	~FViewMode();

	void CreateRasterizerStates();

	ERenderModeIndex CurrentViewMode;
	ID3D11Device* Device;

	TMap<ERenderModeIndex, ID3D11RasterizerState*> RasterizerStates;
};
