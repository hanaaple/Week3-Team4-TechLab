#pragma once
#define _TCHAR_DEFINED
#include <d3d11.h>
#include <memory>

#include "DirectBuffer.h"
#include "Resource/Resource.h"
#include "Core/Container/String.h"
#include "Core/Container/Array.h"
#include "Core/Math/Vector.h"


class FVertexBuffer :
	public FResource<FVertexBuffer> , public FDirectBuffer
{
public:

	template<typename VertexType>																//동적으로 버텍스버퍼를 업데이트 할지 예 :라인 배치
	static std::shared_ptr<FVertexBuffer> Create(const FString& _Name, const TArray<VertexType>& _Data, bool _bIsDynamic = false)
	{
		std::shared_ptr<FVertexBuffer> Res = FVertexBuffer::CreateRes(_Name);

		Res->bIsDynamic = _bIsDynamic;
		if (Res->bIsDynamic == false)
		{
			Res->ResCreate(&_Data[0], sizeof(VertexType), _Data.Num());
		}
		else
		{
			Res->ResCreateDynamic(&_Data[0], sizeof(VertexType), _Data.Num());
		}
	}

	// static std::shared_ptr<FVertexBuffer> Create(const FString&  _Name, size_t _VertexSize, size_t _VertexCount)
	// {
	// 	std::shared_ptr<FVertexBuffer> Res = FVertexBuffer::CreateRes(_Name);
	// 	Res->ResCreate(&_Data[0], sizeof(VertexType), _Data.Num());
	// 	return Res;
	// }

	void Setting() const;

	FVector GetMin() const { return Min; }
	FVector GetMax() const { return Max; }
	
private:
	
	void ResCreate(const void* _Data, size_t _VertexSize, size_t _VertexCount);
	void ResCreateDynamic(const void* _Data, size_t _VertexSize, size_t _VertexCount);


	UINT VertexSize = 0;
	UINT VertexCount = 0;
	UINT Offset = 0;
	bool bIsDynamic = false;
	
	const void* CPUDataPtr = nullptr; //동적 업데이트용 포인터

	FVector Min;
	FVector Max;
};
