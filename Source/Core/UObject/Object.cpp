#include "Core/UObject/Object.h"
#include "Class.h"


UClass* UObject::StaticClass()
{
	static std::unique_ptr<UClass, UClassDeleter> StaticClassInfo = nullptr;
	if (!StaticClassInfo)
	{
		constexpr size_t ClassSize = sizeof(UClass);
		void* RawMemory = FPlatformMemory::Malloc<EAT_Object>(ClassSize);
		UClass* ClassPtr = new(RawMemory) UClass{"UObject", sizeof(UObject), alignof(UObject), nullptr};
		StaticClassInfo = std::unique_ptr<UClass, UClassDeleter>(ClassPtr, UClassDeleter{});
	}
	return StaticClassInfo.get();
}

UObject::UObject()
	: NamePrivate(FString("None"))
	, ClassPrivate(nullptr)
{
}

bool UObject::IsA(const UClass* SomeBase) const
{
	const UClass* ThisClass = GetClass();
	return ThisClass->IsChildOf(SomeBase);
}
