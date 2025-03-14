#pragma once
#include <mutex>

#include "Core/Container/String.h"
#include "Core/Container/Map.h"
template<typename ResourcesType>
class FResource
{
public:
	FResource() {}
	~FResource() {}
	
	// delete Function
	FResource(const FResource& _Other) = delete;
	FResource(FResource&& _Other) noexcept = delete;
	FResource& operator=(const FResource& _Other) = delete;
	FResource& operator=(FResource&& _Other) noexcept = delete;

	static std::shared_ptr<ResourcesType> Find(FString _Name)
	{
	
		std::lock_guard<std::mutex> Lock(NameMutex);
		auto pResult = NameRes.Find(_Name);
		
		return (pResult != nullptr) ? *pResult : std::shared_ptr<ResourcesType>();
	}

protected:
	static std::shared_ptr<ResourcesType> CreateRes(FString _Name)
	{
		std::shared_ptr<ResourcesType> NewRes = std::make_shared<ResourcesType>();

		std::lock_guard<std::mutex> Lock(NameMutex);
		NameRes.Add(_Name, NewRes);
		return NewRes;
	}

private:
	static std::mutex NameMutex;
	static TMap<FString,std::shared_ptr<ResourcesType>> NameRes;

	
};


template<typename ResourcesType>
TMap<FString,std::shared_ptr<ResourcesType>> FResource<ResourcesType>::NameRes;

template<typename ResourcesType>
std::mutex FResource<ResourcesType>::NameMutex;


