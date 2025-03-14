#pragma once
#include "Resource/Resource.h"
#include "DirectBuffer.h"
#include "Core/Container/String.h"
#include "Core/Container/Array.h"
#include <d3d11.h>
#include "Primitive/PrimitiveVertices.h"


class FVertexBuffer :
	public FResource<FVertexBuffer> , public FDirectBuffer
{
public:
	static D3D11_INPUT_ELEMENT_DESC Layout[]; 



	
	template<typename VertexType>
static std::shared_ptr<FVertexBuffer> Create(FString _Name, const TArray<VertexType>& _Data)
	{
		std::shared_ptr<FVertexBuffer> Res = FVertexBuffer::CreateRes(_Name);
		Res->ResCreate(&_Data[0], sizeof(VertexType), _Data.Num());
		return Res;
	}

	void Setting() const;
	
private:
	
	void ResCreate(const void* _Data, size_t _VertexSize, size_t _VertexCount);


	UINT VertexSize = 0;
	UINT VertexCount = 0;
	UINT Offset = 0;

	
	
};
