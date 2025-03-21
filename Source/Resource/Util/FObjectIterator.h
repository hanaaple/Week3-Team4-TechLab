#pragma once
#include <concepts>

#include "Core/Engine.h"
#include "Core/Container/Map.h"
#include "Core/UObject/Object.h"

// TArray용

template<class T>
	requires std::derived_from<T, UObject>
class TObjectIterator
{
public:
	TArray<std::shared_ptr<UObject>> GObjects;

	TObjectIterator(int Idx = 0)
	{
		GObjects = TArray<std::shared_ptr<UObject>>();
		for (const auto& UObjectPair : UEngine::Get().GObjects)
		{
			GObjects.Add(UObjectPair.Value);
		}
		Index = Idx;
		Advance();
	}
	~TObjectIterator() = default;

	T* operator*() const { return (T*)GetUObject(); }
	T* operator->() const { return (T*)GetUObject(); }


	explicit operator bool() const {return GObjects.IsValidIndex(Index); }
	bool operator !() const { return !(bool)*this; }
	
	bool operator!=(const TObjectIterator& Other) const { return Index != Other.Index; }
	bool operator==(const TObjectIterator& Other) const { return Index == Other.Index; }

	TObjectIterator& operator++()
	{
		Advance();
		return *this;
	}
	TObjectIterator operator++(int)
	{
		TObjectIterator temp = *this; // 현재 상태 저장
		Advance();
		return temp;
	}
	
	TObjectIterator<T> begin() { return TObjectIterator<T>(-1); }
	TObjectIterator<T> end() { return TObjectIterator<T>(GObjects.Num()); }
private:
	bool Advance()
	{
		while (++Index < GObjects.Num())
		{
			if (GObjects[Index]->IsA<T>())
			{
				return true;
			}
		}
		return false;
	}

	UObject* GetUObject() const 
	{ 
		return GObjects[Index].get();
	}
	
private:
	uint32 Index;
};


// template<class T>
// 	requires std::derived_from<T, UObject>
// class TObjectIterator
// {
// public:
// 	using Type = std::shared_ptr<T>;
// 	using MapType = TMap<uint32, Type>;
// 	using SizeType = typename MapType::SizeType;
// 	using IteratorType = typename MapType::iterator; 
// 	using ConstIteratorType = typename MapType::const_iterator;
//
// private:
// 	class Iterator
// 	{
// 	private:
// 		IteratorType InnerIt;
// 	public:
// 		Iterator(IteratorType it) : InnerIt(std::move(it)) {}
// 		Type& operator*() { return (InnerIt->second); }
// 		Type* operator->() { return &InnerIt->second; }
// 		Iterator& operator++() { ++InnerIt; return *this; }
// 		bool operator!=(const Iterator& other) const { return InnerIt != other.InnerIt; }
// 		// bool Advance()
// 		// {
// 		// 	while (++InnerIt != UEngine::Get().GObjects.end())
// 		// 	{
// 		// 		if (InnerIt->second->IsA<T>())
// 		// 		{
// 		// 			return true;
// 		// 		}
// 		// 	}
// 		// 	return false;
// 		// }
// 	};
// 	// class ConstIterator
// 	// {
// 	// private:
// 	// 	ConstIteratorType InnerIt;
// 	// public:
// 	// 	ConstIterator(ConstIteratorType it) : InnerIt(it) {}
// 	// 	const Type& operator*() const { return InnerIt->second; }
// 	// 	const Type* operator->() const { return &InnerIt->second; }
// 	// 	ConstIterator& operator++() { Advance(); return *this; }
// 	// 	bool operator!=(const ConstIterator& other) const { return InnerIt != other.InnerIt; }
// 	//
// 	// 	bool Advance()
// 	// 	{
// 	// 		while (++InnerIt != GObjects.end())
// 	// 		{
// 	// 			if (InnerIt->second->IsA<T>())
// 	// 			{
// 	// 				return true;
// 	// 			}
// 	// 		}
// 	// 		return false;
// 	// 	}
// 	// };
//
// public:
// 	
// 	Iterator begin() noexcept { return Iterator(UEngine::Get().GObjects.AAA()); }
// 	Iterator end() noexcept { return Iterator(UEngine::Get().GObjects.DDD()); }
// 	// ConstIterator begin() const noexcept { return ConstIterator(GObjects.begin()); }
// 	// ConstIterator end() const noexcept { return ConstIterator(GObjects.end()); }
// };