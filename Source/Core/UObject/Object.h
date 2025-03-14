#pragma once
#include <memory>

#include "NameTypes.h"
#include "ObjectMacros.h"
#include "Core/Container/String.h"
#include "Core/HAL/PlatformType.h"


// TODO: RTTI 구현하면 enable_shared_from_this 제거
class UObject : public std::enable_shared_from_this<UObject>
{
	DECLARE_CLASS(UObject, UObject)

private:
	friend class FObjectFactory;

	FName NamePrivate;

	uint32 UUID = 0;
	uint32 InternalIndex; // Index of GUObjectArray

public:
	UObject();
	virtual ~UObject();

public:
	FName GetFName() const { return NamePrivate; }
	FString GetName() const { return GetFName().ToString(); }

	uint32 GetUUID() const { return UUID; }
	uint32 GetInternalIndex() const { return InternalIndex; }

	/** this가 SomeBase인지, SomeBase의 자식 클래스인지 확인합니다. */
	FORCEINLINE bool IsA(UClass* SomeBase) const
	{
		return true;
	}

	template<typename T>
	bool IsA() const
	{
		return IsA(T::StaticClass());
	}
};
