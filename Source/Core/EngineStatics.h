#pragma once
#include "HAL/PlatformType.h"


class UEngineStatics
{
    static uint32 NextUUID;
	friend class JsonSaveHelper;

public:
    static uint32 GenUUID()
    {
        return NextUUID++;
    }
};
