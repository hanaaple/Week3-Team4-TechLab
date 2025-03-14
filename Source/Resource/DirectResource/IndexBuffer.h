#pragma once

#include <memory>

#include "Resource/Resource.h"
#include "DirectBuffer.h"
#include "Core/Container/String.h"
#include "Core/Container/Array.h"
#include "Primitive/PrimitiveVertices.h"
#include "Core/HAL/PlatformType.h"


class FIndexBuffer :
	public FResource<FIndexBuffer> , public FDirectBuffer

{
public:
	
static std::shared_ptr<FIndexBuffer> Create(FString _Name, const TArray<uint32>& _Data)
	{
		std::shared_ptr<FIndexBuffer> Res = FIndexBuffer::CreateRes(_Name);
		Res->ResCreate(&_Data[0], _Data.Num());
		return Res;
	}

	void Setting() const;
	
	inline uint32 GetIndexCount() 
	{
		return IndexCount;
	}
	
private:
	
	void ResCreate(const void* _Data, size_t _IndexCount);

	DXGI_FORMAT Format = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
	uint32 IndexSize = sizeof(uint32);
	uint32 IndexCount = 0;
	uint32 Offset = 0;

	
	
};
