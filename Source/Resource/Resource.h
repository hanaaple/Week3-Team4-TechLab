#pragma once
#include <mutex>

#include "Core/Container/String.h"
#include "Core/Container/Map.h"
#include "Debug/DebugConsole.h"

template<typename ResourcesType>
class FResource 
{
public:
	FResource() {}

	virtual ~FResource()
	{
	}


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

	void SetName(FString _Name)
	{
		Name = _Name;
	}

	FString GetName()
	{
		return Name;
	}

protected:
	static std::shared_ptr<ResourcesType>  CreateRes(const FString&  _Name)
	{
		std::shared_ptr<ResourcesType> NewRes = std::make_shared<ResourcesType>();

		std::lock_guard<std::mutex> Lock(NameMutex);
		NewRes->SetName(_Name);
		NameRes.Add(_Name, NewRes);
		return NewRes;
	}

private:
	static std::mutex NameMutex;
	static TMap<FString,std::shared_ptr<ResourcesType>> NameRes;

	
	FString Name;

	
};




template<typename ResourcesType>
TMap<FString,std::shared_ptr<ResourcesType>> FResource<ResourcesType>::NameRes;

template<typename ResourcesType>
std::mutex FResource<ResourcesType>::NameMutex;


