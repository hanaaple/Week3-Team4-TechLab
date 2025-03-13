#include "NameTypes.h"

#include <cwchar>
#include "Core/AbstractClass/Singleton.h"
#include "Core/Container/Set.h"
#include "Core/Container/String.h"


enum ENameCase : uint8
{
	IgnoreCase,   // 대소문자 무시
	CaseSensitive // 대소문자 구분
};

struct FNameStringView
{
	FNameStringView() : Data(nullptr), Len(0), bIsWide(false) {}
	FNameStringView(const ANSICHAR* Str, uint32 InLen) : Ansi(Str), Len(InLen), bIsWide(false) {}
	FNameStringView(const WIDECHAR* Str, uint32 InLen) : Wide(Str), Len(InLen), bIsWide(true) {}
	FNameStringView(const void* InData, uint32 InLen, bool bInIsWide) : Data(InData), Len(InLen), bIsWide(bInIsWide) {}

	union
	{
		const void* Data;
		const ANSICHAR* Ansi;
		const WIDECHAR* Wide;
	};

	uint32 Len;
	bool bIsWide;

	bool IsAnsi() const { return !bIsWide; }
};


struct FNameEntryId
{
	uint32 Value;  // 비교 문자열이 있는 해시

	bool operator==(const FNameEntryId& Other) const
	{
		return Value == Other.Value;
	}

	bool operator!=(const FNameEntryId& Other) const
	{
		return !(*this == Other);
	}
};

struct FNameEntryHeader
{
	uint16 IsWide : 1; // wchar인지 여부
	uint16 Len : 15;   // FName의 길이 0 ~ 32767
};

struct FNameEntry
{
	static constexpr uint32 NAME_SIZE = 1024; // FName에 저장될 수 있는 최대 길이

	FNameEntryId ComparisonId; // 비교 문자열이 있는 해시
	FNameEntryHeader Header;   // FName의 정보

	union
	{
		ANSICHAR AnsiName[NAME_SIZE];
		WIDECHAR WideName[NAME_SIZE];
	};

	void StoreName(const ANSICHAR* InName, uint32 Len)
	{
		memcpy(AnsiName, InName, sizeof(ANSICHAR) * Len);
	}

	void StoreName(const WIDECHAR* InName, uint32 Len)
	{
		memcpy(WideName, InName, sizeof(WIDECHAR) * Len);
	}

	bool operator==(const FNameEntry& Other) const
	{
		return ComparisonId == Other.ComparisonId;
	}

	bool operator!=(const FNameEntry& Other) const
	{
		return !(*this == Other);
	}
};

template <typename CharType>
uint32 HashString(const CharType* Str)
{
	// djb2 문자열 해싱 알고리즘
	uint32 Hash = 5381;
	while (*Str)
	{
		Hash = ((Hash << 5) + Hash) + *Str;
		++Str;
	}
	return Hash;
}

template <typename CharType>
uint32 HashStringLower(const CharType* Str, uint32 InLen)
{
	CharType LowerStr[FNameEntry::NAME_SIZE];
	if constexpr (std::is_same_v<CharType, wchar_t>)
	{
		for (uint32 i = 0; i < InLen; i++)
		{
			LowerStr[i] = towlower(Str[i]);
		}
		LowerStr[InLen] = '\0';
	}
	else
	{
		for (uint32 i = 0; i < InLen; ++i)
		{
			LowerStr[i] = tolower(Str[i]);
		}
		LowerStr[InLen] = '\0';
	}
	return HashString(LowerStr);
}

template <ENameCase Sensitivity>
uint32 HashName(FNameStringView InName);

template <>
uint32 HashName<IgnoreCase>(FNameStringView InName)
{
	return InName.IsAnsi() ? HashStringLower(InName.Ansi, InName.Len) : HashStringLower(InName.Wide, InName.Len);
}

template <>
uint32 HashName<CaseSensitive>(FNameStringView InName)
{
	return InName.IsAnsi() ? HashString(InName.Ansi) : HashString(InName.Wide);
}

template <ENameCase Sensitivity>
struct FNameValue
{
	explicit FNameValue(FNameStringView InName)
		: Name(InName)
		, Hash(HashName<Sensitivity>(InName))
	{}

	FNameStringView Name;
	uint32 Hash;
	FNameEntryId ComparisonId = {};
};

using FNameComparisonValue = FNameValue<IgnoreCase>;
using FNameDisplayValue = FNameValue<CaseSensitive>;

struct FNameHash
{
	uint32 operator()(const FNameEntry& InName) const
	{
		return InName.Header.IsWide ? HashString(InName.WideName) : HashString(InName.AnsiName);
	}
};

struct FNamePool : public TSingleton<FNamePool>
{
private:
	TSet<FNameEntry, FNameHash> ComparisonPool; // IgnoreCase

public:
	/** Hash로 값을 가져옵니다. */
	FNameEntry Resolve(uint32 Hash) const
	{
		const size_t BucketCount = ComparisonPool.PrivateSet.bucket_count();
		const uint32 ItemIndex = Hash % BucketCount;
		return *ComparisonPool.PrivateSet.begin(ItemIndex);
	}

	FNameEntryId FindOrStoreString(const FNameStringView& Name)
	{
		auto& ComparisonPoolSet = ComparisonPool.PrivateSet;
		const size_t BucketCount = ComparisonPoolSet.bucket_count();

		const FNameComparisonValue ComparisonValue{Name};
		const uint32 ComparisonIdx = ComparisonValue.Hash % BucketCount;
		if (ComparisonPoolSet.begin(ComparisonIdx) != ComparisonPoolSet.end())
		{
			return ComparisonPoolSet.begin(ComparisonIdx)->ComparisonId;
		}

		FNameEntry Entry;
		Entry.ComparisonId = { ComparisonValue.Hash };
		Entry.Header = {
			.IsWide = Name.bIsWide,
			.Len = static_cast<uint16>(Name.Len)
		};
		if (Name.bIsWide)
		{
			Entry.StoreName(Name.Wide, Name.Len);
		}
		else
		{
			Entry.StoreName(Name.Ansi, Name.Len);
		}
		ComparisonPoolSet.insert(Entry);
		return Entry.ComparisonId;
	}
};

FName::FName(const WIDECHAR* Name)
	: ComparisonIndex(
		FNamePool::Get().FindOrStoreString({
			Name,
			static_cast<uint16>(wcslen(Name))
		}).Value
	)
{
}

FName::FName(const ANSICHAR* Name)
	: ComparisonIndex(
		FNamePool::Get().FindOrStoreString({
			Name,
			static_cast<uint16>(strlen(Name))
		}).Value
	)
{
}

FName::FName(const FString& Name)
	: ComparisonIndex(
		FNamePool::Get().FindOrStoreString({
			*Name,
			static_cast<uint16>(Name.Len())
		}).Value
	)
{
}

FString FName::ToString() const
{
	// TODO: WIDECHAR에 대응 해야함
	FNameEntry Entry = FNamePool::Get().Resolve(ComparisonIndex);
	return {
		// Entry.Header.IsWide ? Entry.WideName : Entry.AnsiName
		Entry.AnsiName
	};
}

// bool FName::Equals(const FName& Other) const
// {
// 	return {}; // TODO: Implementation this
// }

bool FName::operator==(const FName& Other) const
{
	return ComparisonIndex == Other.ComparisonIndex;
}
