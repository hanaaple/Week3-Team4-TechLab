#pragma once
#define _TCHAR_DEFINED
#include <d3d11.h>
#include <memory>

#include "Resource/Resource.h"
#include "Core/Container/String.h"


class FDepthStencilState:
public FResource<FDepthStencilState> 
{
public:
	// constrcuter destructer
	FDepthStencilState();
	~FDepthStencilState();

	static std::shared_ptr<FDepthStencilState> Create(const FString& _Name, const D3D11_DEPTH_STENCIL_DESC& _Desc)
	{
		std::shared_ptr<FDepthStencilState> Res = CreateRes(_Name);
		Res->ResCreate(_Desc);
		return Res;
	}

	void Setting();

protected:
	void ResCreate(const D3D11_DEPTH_STENCIL_DESC& _Desc);

private:
	D3D11_DEPTH_STENCIL_DESC Desc = {};
	ID3D11DepthStencilState* State = nullptr;
};
