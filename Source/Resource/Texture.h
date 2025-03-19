#pragma once
#define _TCHAR_DEFINED
#include <d3d11.h>

#include "Resource/Resource.h"
#include "Core/Container/String.h"
#include "Core/Container/Array.h"
#include "Core/hal/PlatformType.h"


class FTexture :
	public FResource<FTexture> 
{
public:

	// constrcuter destructer
	FTexture();
	~FTexture();

	// static std::shared_ptr<FTexture> Load(std::string_view _Path, 
	// D3D11_FILTER _Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR,
	// D3D11_TEXTURE_ADDRESS_MODE _Address = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP)
	// {
	// 	return Load(Path.GetStringPath(), Path.GetFileName(), _Filter, _Address);
	//}

	static std::shared_ptr<FTexture> Load(const FString& _Path, const FString& _Name)
		// D3D11_FILTER _Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		// D3D11_TEXTURE_ADDRESS_MODE _Address = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP)
	{
		std::shared_ptr<FTexture> NewRes = CreateRes(_Name);
		NewRes->ResLoad(_Path);
		// NewRes->Filter = _Filter;
		// NewRes->Address = _Address;
		return NewRes;
	}
	
	//리소스 등록
	static std::shared_ptr<FTexture> Create(const FString& _Name, ID3D11Texture2D* _Res)
	{
		std::shared_ptr<FTexture> NewRes = CreateRes(_Name);
		NewRes->ResCreate(_Res);
		return NewRes;
	}
	
	//텍스쳐 생성
	static std::shared_ptr<FTexture> Create(const FString& _Name,const D3D11_TEXTURE2D_DESC& _Desc)
	{
		std::shared_ptr<FTexture> NewRes = CreateRes(_Name);
		NewRes->ResCreate(_Desc);
		return NewRes;
	}

	void VSSetting(UINT _Slot);
	void PSSetting(UINT _Slot);
	void CSSetting(UINT _Slot);

	void VSReset(UINT _Slot);
	void PSReset(UINT _Slot);
	void CSReset(UINT _Slot);

	// 랜더타겟 세팅용
	void CreateRenderTargetView();
	// 쉐이더 세팅용
	void CreateShaderResourceView();
	// 깊버거 세팅용
	void CreateDepthStencilView();

	ID3D11Texture2D* GetTexture2D() const 
	{
		return Texture2D;
	}

	ID3D11RenderTargetView* GetRTV() 
	{
		return RTV;
	}

	
	ID3D11DepthStencilView*	GetDSV() 
	{
		return DSV;
	}
	ID3D11ShaderResourceView* GetSRV() 
	{
		return SRV;
	}
	
private:
	D3D11_TEXTURE2D_DESC Desc;

	ID3D11Texture2D* Texture2D = nullptr;

	ID3D11RenderTargetView* RTV = nullptr; // 수정대상으로 삼거나 수정할수 있는 권한.
	ID3D11ShaderResourceView* SRV = nullptr; // 쉐이더에 세팅해줄수 있는 권한
	ID3D11DepthStencilView* DSV = nullptr; // 깊이버퍼를 세팅해줄수 있는 권한
	

	// D3D11_FILTER Filter;
	// D3D11_TEXTURE_ADDRESS_MODE Address;

	void ResLoad(const FString& _Path);
	void ResCreate(const D3D11_TEXTURE2D_DESC& Desc);
	void ResCreate(ID3D11Texture2D* _Res);
};
