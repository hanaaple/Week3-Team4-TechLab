#pragma once
#include <cassert>
#include "NameTypes.h"


class UClass
{
private:
	FName ClassName;
	int32 ClassSize;

public:
	UClass();

	bool IsChildOf(const UClass* SomeBase) const
	{
		assert(this);
		if (!SomeBase) return false;

		for (const UClass* TempClass=this; TempClass; TempClass=TempClass->GetSuperClass())
		{
			if (TempClass == SomeBase)
			{
				return true;
			}
		}
		return false;
	}

	UClass* GetSuperClass() const
	{
		
	}
};
