#pragma once
#include <unordered_set>
#include "Array.h"
#include "ContainerAllocator.h"


template <typename T, typename Hasher = std::hash<T>, typename Allocator = FDefaultAllocator<T>>
class TSet
{
private:
    using SetType = std::unordered_set<T, Hasher, std::equal_to<>, Allocator>;
    SetType PrivateSet;

	friend struct FNamePool;

public:
    using SizeType = typename Allocator::SizeType;
    using Iterator = typename SetType::iterator;
    using ConstIterator = typename SetType::const_iterator;

    // 기본 생성자
    TSet() = default;

    // Iterator 관련 메서드
    Iterator begin() noexcept { return PrivateSet.begin(); }
    Iterator end() noexcept { return PrivateSet.end(); }
    ConstIterator begin() const noexcept { return PrivateSet.begin(); }
    ConstIterator end() const noexcept { return PrivateSet.end(); }

    // Add
    int32 Add(const T& Item) { return Emplace(Item); }
    int32 Add(T&& Item) { return Emplace(std::move(Item)); }

    /**
     * r-value를 받아 값을 새로 만들어 Set에 추가합니다.
     * @tparam ArgsType TSet<T>의 T부분
     * @param Args Set에 추가될 인자 (r-value)
     * @return 새로 추가된 Element의 Index, 이미 존재하는 경우 기존 Element의 Index를 반환
     */
    template<typename ArgsType = T>
    int32 Emplace(ArgsType&& Args) 
    { 
        auto iter = PrivateSet.emplace(std::forward<ArgsType>(Args));
    	return std::distance(PrivateSet.begin(), iter.first);
    }

    // Num (개수)
    SizeType Num() const { return static_cast<SizeType>(PrivateSet.size()); }

    // Find
    Iterator Find(const T& Item) { return PrivateSet.find(Item); }
    ConstIterator Find(const T& Item) const { return PrivateSet.find(Item); }

	// Contains
	bool Contains(const T& Item) const { return PrivateSet.contains(Item); }

    // Array (TArray로 반환)
    TArray<T, Allocator> Array() const
    {
        TArray<T, Allocator> Result;
        Result.Reserve(Num());
        for (const auto& Item : PrivateSet)
        {
            Result.Add(Item);
        }
        return Result;
    }

    // Remove
    SizeType Remove(const T& Item) { return static_cast<SizeType>(PrivateSet.erase(Item)); }

    // Empty
    void Empty() { PrivateSet.clear(); }

    // IsEmpty
    bool IsEmpty() const { return PrivateSet.empty(); }
};
