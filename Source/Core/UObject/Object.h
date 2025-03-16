#pragma once
#include <memory>

#include "NameTypes.h"
#include "Core/Container/String.h"
#include "Core/HAL/PlatformType.h"


class UClass;

class UObject : public std::enable_shared_from_this<UObject>
{
private:
	UObject(const UObject&) = delete;
	UObject& operator=(const UObject&) = delete;
	UObject(UObject&&) = delete;
	UObject& operator=(UObject&&) = delete;

public:
	using Super = UObject;
	using ThisClass = UObject;

	static UClass* StaticClass();

private:
	friend class FObjectFactory;
	friend class UClass;

	/** Object의 Instance Name */
	FName NamePrivate;
	UClass* ClassPrivate;

	uint32 UUID = 0;
	uint32 InternalIndex = std::numeric_limits<uint32>::max(); // Index of GUObjectArray

public:
	UObject();
	virtual ~UObject() = default;

public:
	FName GetFName() const { return NamePrivate; }
	FString GetName() const { return GetFName().ToString(); }

	uint32 GetUUID() const { return UUID; }
	uint32 GetInternalIndex() const { return InternalIndex; }

	UClass* GetClass() const { return ClassPrivate; }

	/** this가 SomeBase인지, SomeBase의 자식 클래스인지 확인합니다. */
	bool IsA(const UClass* SomeBase) const;

	template<typename T>
	bool IsA() const
	{
		return IsA(T::StaticClass());
	}
};
