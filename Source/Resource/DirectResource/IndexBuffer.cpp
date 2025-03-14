#include "IndexBuffer.h"
#include "Core/Engine.h"
#include "Core/Rendering/URenderer.h"
#include "Debug/DebugConsole.h"
#include "d3d11.h"

#include "Core/Rendering/FDevice.h"

void FIndexBuffer::Setting() const
{
	// ID3D11Buffer* Arr[1];

	if (nullptr == Buffer)
	{
		UE_LOG("Error: FIndexBuffer Setting Failed");
	}


	// 버텍스버퍼를 여러개 넣어줄수 있다.
	FDevice::Get().GetDeviceContext()->IASetIndexBuffer(Buffer, Format, Offset);
}

void FIndexBuffer::ResCreate(const void* _Data, size_t _IndexCount)
{
	IndexCount = static_cast<UINT>(_IndexCount);

	BufferInfo.BindFlags = D3D11_BIND_INDEX_BUFFER;
	BufferInfo.ByteWidth = static_cast<UINT>(IndexSize * IndexCount);
	BufferInfo.CPUAccessFlags = 0;
	BufferInfo.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA Data;
	Data.pSysMem = _Data;

	if (S_OK != FDevice::Get().GetDevice()->CreateBuffer(&BufferInfo, &Data, &Buffer))
	{
		UE_LOG("Error: FIndexBuffer Create Failed") ;
		return;
	}
}
