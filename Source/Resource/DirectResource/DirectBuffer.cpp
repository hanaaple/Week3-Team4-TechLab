#include "DirectBuffer.h"

FDirectBuffer::FDirectBuffer()
{
}

FDirectBuffer::~FDirectBuffer()
{
	BufferRelease();
}

void FDirectBuffer::BufferRelease()
{
	if (nullptr != Buffer)
	{
		Buffer->Release();
		Buffer = nullptr;
	}
}
