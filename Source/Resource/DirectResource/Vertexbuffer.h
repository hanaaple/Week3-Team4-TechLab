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
	template<typename VertexType>
	static std::shared_ptr<FVertexBuffer> Create(const FString&  _Name, const TArray<VertexType>& _Data)
	{
		std::shared_ptr<FVertexBuffer> Res = FVertexBuffer::CreateRes(_Name);
		for (const auto& Data : _Data)
		{
			Res->Min = FVector::Min(Res->Min, FVector(Data.X, Data.Y, Data.Z));
			Res->Max = FVector::Max(Res->Max, FVector(Data.X, Data.Y, Data.Z));
		}
		Res->ResCreate(&_Data[0], sizeof(VertexType), _Data.Num());
		return Res;
	}

	void Setting() const;

	FVector GetMin() const { return Min; }
	FVector GetMax() const { return Max; }
	
private:
	
	void ResCreate(const void* _Data, size_t _VertexSize, size_t _VertexCount);


	UINT VertexSize = 0;
	UINT VertexCount = 0;
	UINT Offset = 0;

	FVector Min;
	FVector Max;
};
