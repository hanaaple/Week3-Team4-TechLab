#include "StaticMesh.h"
#include "Core/Engine.h"
#include "Core/Rendering/FDevice.h"


void UStaticMesh::Setting()
{
	if (nullptr == VertexBuffer)
	{
		MsgBoxAssert("매쉬가 세팅되어 있지 않습니다.");
		return;
	}
	VertexBuffer->Setting();

	FDevice::Get().GetDeviceContext()->IASetPrimitiveTopology(Topology);

	if (nullptr == IndexBuffer)
	{
		MsgBoxAssert("매쉬가 세팅되어 있지 않습니다.");
		return;
	}
	IndexBuffer->Setting();
}

void UStaticMesh::Draw()
{
	
	FDevice::Get().GetDeviceContext()->DrawIndexed(IndexBuffer->GetIndexCount(), 0, 0);
}

