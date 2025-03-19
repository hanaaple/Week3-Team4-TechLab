#pragma once
#include <mutex>

#include "Core/Container/String.h"
#include "Core/Container/Map.h"
#include "Debug/DebugConsole.h"

template <typename ResourcesType>
class FResource
{
public:
	FResource() = default;
	virtual ~FResource() = default;

	// delete Function
	FResource(const FResource& Other) = delete;
	FResource(FResource&& Other) noexcept = delete;
	FResource& operator=(const FResource& Other) = delete;
	FResource& operator=(FResource&& Other) noexcept = delete;

	static std::shared_ptr<ResourcesType> Find(const FString& InName)
	{
		std::lock_guard Lock(NameMutex);
		auto pResult = NameRes.Find(InName);

		return (pResult != nullptr) ? *pResult : nullptr;
	}

	static void Release(FString _Name)
	{
		std::lock_guard<std::mutex> Lock(NameMutex);

		NameRes.Remove(_Name);
		//auto pResult = NameRes.Find(_Name);

	/*	if (pResult == nullptr)
		{
			MsgBoxAssert("존재하지도 않는 리소스를 지우려고 했습니다.");
			return;
		}*/
	}


	static void AllResourcesRelease()
	{
		{
			std::lock_guard<std::mutex> Lock(NameMutex);
			NameRes.Empty();
		}
	}

	void SetName(const FString& InName)
	{
		Name = InName;
	}

	FString GetName()
	{
		return Name;
	}

protected:
	static std::shared_ptr<ResourcesType> CreateRes(const FString& InName)
	{
		std::shared_ptr<ResourcesType> NewRes = std::make_shared<ResourcesType>();

		std::lock_guard Lock(NameMutex);
		NewRes->SetName(InName);
		NameRes.Add(InName, NewRes);
		return NewRes;
	}

private:
	static std::mutex NameMutex;
	static TMap<FString, std::shared_ptr<ResourcesType>> NameRes;

	FString Name;
};


template <typename ResourcesType>
TMap<FString, std::shared_ptr<ResourcesType>> FResource<ResourcesType>::NameRes;

template <typename ResourcesType>
std::mutex FResource<ResourcesType>::NameMutex;
