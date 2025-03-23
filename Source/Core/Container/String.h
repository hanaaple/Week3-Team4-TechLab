#pragma once

#include <string>
#include "CString.h"
#include "ContainerAllocator.h"
#include "Core/HAL/PlatformType.h"

/*
# TCHAR가 ANSICHAR인 경우
1. const ANSICHAR* 로 FString 생성
2. std::string에서 FString 생성

# TCHAR가 WIDECHAR인 경우
1. const ANSICHAR* 로 FString 생성
1. const WIDECHAR* 로 FString 생성
2. std::wstring에서 FString 생성
3. std::string에서 FString 생성
*/

enum : int8 { INDEX_NONE = -1 };

/** Determines case sensitivity options for string comparisons. */
namespace ESearchCase
{
	enum Type : uint8
	{
	    /** Case sensitive. Upper/lower casing must match for strings to be considered equal. */
	    CaseSensitive,

	    /** Ignore case. Upper/lower casing does not matter when making a comparison. */
	    IgnoreCase,
	};
};

/** Determines search direction for string operations. */
namespace ESearchDir
{
	enum Type : uint8
	{
	    /** Search from the start, moving forward through the string. */
	    FromStart,

	    /** Search from the end, moving backward through the string. */
	    FromEnd,
	};
}

class FString
{
public:
	static const size_t npos = -1;

private:
	using ElementType = TCHAR;
    using BaseStringType = std::basic_string<
        ElementType,
        std::char_traits<ElementType>,
        FDefaultAllocator<ElementType>
    >;

    BaseStringType PrivateString;

	friend struct std::hash<FString>;

public:
    FString() = default;
    ~FString() = default;

	FString(const FString&) = default;
    FString& operator=(const FString&) = default;
    FString(FString&&) = default;
    FString& operator=(FString&&) = default;

    FString(BaseStringType InString) : PrivateString(std::move(InString)) {}

public:
	static char* TCHAR_TO_ANSI(const TCHAR*);
private:
	// WIDECHAR 사용처에서 사용할 수 있도록
	static std::wstring ConvertToWideChar(const ANSICHAR* NarrowStr);
	// ANSICHAR 사용처에서 사용할 수 있도록
	static std::string ConvertToMultibyte(const WIDECHAR* WideStr);

#if USE_WIDECHAR
public:
    FString(const std::wstring& InString) : PrivateString(InString) {}
    FString(const std::string& InString) : PrivateString(ConvertToWideChar(InString.c_str())) {}
    FString(const WIDECHAR* InString) : PrivateString(InString) {}
    FString(const ANSICHAR* InString) : PrivateString(ConvertToWideChar(InString)) {}
#else
public:
    FString(const std::string& InString) : PrivateString(InString) {}
    FString(const ANSICHAR* InString) : PrivateString(InString) {}
#endif

	// 반드시 const char*를 반환하는 함수
	FORCEINLINE const char* c_char() const;
	FORCEINLINE const wchar_t* c_wchar() const;

	/** Use c_char() or c_wchar() instead. */
//#if USE_WIDECHAR
//	FORCEINLINE std::string ToAnsiString() const
//	{
//		// Wide 문자열을 UTF-8 기반의 narrow 문자열로 변환
//		if (PrivateString.empty())
//		{
//			return std::string();
//		}
//		int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, PrivateString.c_str(), -1, nullptr, 0, nullptr, nullptr);
//		if (sizeNeeded <= 0)
//		{
//			return std::string();
//		}
//		std::string result(sizeNeeded, 0);
//		WideCharToMultiByte(CP_UTF8, 0, PrivateString.c_str(), -1, &result[0], sizeNeeded, nullptr, nullptr);
//		return result;
//	}
//#else
//	FORCEINLINE std::wstring ToWideString() const
//	{
//#if USE_WIDECHAR
//		return PrivateString;
//#else
//		// Narrow 문자열을 UTF-8로 가정하고 wide 문자열로 변환
//		if (PrivateString.empty())
//		{
//			return std::wstring();
//		}
//		int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, PrivateString.c_str(), -1, nullptr, 0);
//		if (sizeNeeded <= 0)
//		{
//			return std::wstring();
//		}
//		std::wstring wstr(sizeNeeded, 0);
//		MultiByteToWideChar(CP_UTF8, 0, PrivateString.c_str(), -1, &wstr[0], sizeNeeded);
//		return wstr;
//#endif
//	}
//#endif

	template <typename Number>
		requires std::is_arithmetic_v<Number>
    static FString FromInt(Number Num);

    static FString SanitizeFloat(float InFloat);

	static float ToFloat(const FString& InString);

	// findlastof
	FORCEINLINE size_t FindLastOf(const ElementType* Char) const;

public:
    FORCEINLINE int32 Len() const;
    FORCEINLINE bool IsEmpty() const;

    /** 배열의 모든 요소를 지웁니다. */
    void Empty();

    /**
     * 문자열이 서로 같은지 비교합니다.
     * @param Other 비교할 String
     * @param SearchCase 대소문자 구분
     * @return 같은지 여부
     */
    bool Equals(const FString& Other, ESearchCase::Type SearchCase = ESearchCase::CaseSensitive) const;

    /**
     * 문자열이 겹치는지 확인합니다.
     * @param SubStr 찾을 문자열
     * @param SearchCase 대소문자 구분
     * @param SearchDir 찾을 방향
     * @return 문자열 겹침 여부
     */
    bool Contains(
        const FString& SubStr, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase,
        ESearchDir::Type SearchDir = ESearchDir::FromStart
    ) const;

    /**
     * 문자열을 찾아 Index를 반홥합니다.
     * @param SubStr 찾을 문자열
     * @param SearchCase 대소문자 구분
     * @param SearchDir 찾을 방향
     * @param StartPosition 시작 위치
     * @return 찾은 문자열의 Index를 반환합니다. 찾지 못하면 -1
     */
    int32 Find(
        const FString& SubStr, ESearchCase::Type SearchCase = ESearchCase::IgnoreCase,
        ESearchDir::Type SearchDir = ESearchDir::FromStart, int32 StartPosition = -1
    ) const;

	/**
	 * 대소문자 구분 없이 문자열을 지정된 길이만큼 비교합니다.
	 * @param Other 비교할 String
	 * @param Count 비교할 길이
	 * @return 비교 결과
	 */
	int Strnicmp(const FString& Other, const size_t Count) const;

	/**
	 * 문자열의 n번째 이후의 부분 문자열을 반환합니다.
	 * @param Pos 시작 위치
	 * @param Count 길이 (기본값: npos)
	 * @return 부분 문자열
	 */
	FString Substr(const size_t Pos, const size_t Count = npos) const;

	void RemoveAt(int32 Index, int32 Count, bool bAllowShrinking);

	/** 문자열의 첫 번째 문자를 반환합니다. */
	FORCEINLINE ElementType& Front();
	FORCEINLINE const ElementType& Front() const;

	/** 문자열의 마지막 문자를 반환합니다. */
	FORCEINLINE ElementType& Back();
	FORCEINLINE const ElementType& Back() const;

	/** 문자열의 마지막 문자를 제거합니다. */
	void PopBack();

	/** 문자열의 지정된 위치에서 지정된 길이만큼 제거합니다. */
	void RemoveAt(const size_t Pos, const size_t Count = npos);
	void RemoveAt(BaseStringType::iterator It, const size_t Count = npos);

	/** 문자열의 시작 반복자를 반환합니다. */
	FORCEINLINE BaseStringType::iterator Begin();
	FORCEINLINE BaseStringType::const_iterator Begin() const;

	/** 문자열을 대문자로 변환합니다. */
	FString ToUpper() const;


	const FString::ElementType* GetData() const;

public:
    /** ElementType* 로 반환하는 연산자 */
    FORCEINLINE const ElementType* operator*() const;

    // FORCEINLINE FString operator+(const FString& SubStr) const;
	FORCEINLINE FString& operator+=(const FString& SubStr);
	FORCEINLINE FString& operator+=(const ElementType* Rhs);
	FORCEINLINE FString operator+(const FString& SubStr);
	FORCEINLINE FString operator+(const ElementType* Rhs);
	FORCEINLINE friend FString operator+(const FString& Lhs, const FString& Rhs);

    FORCEINLINE bool operator==(const FString& Rhs) const;
    FORCEINLINE bool operator==(const ElementType* Rhs) const;

	/** n번째 위치의 문자를 가져오는 연산자 오버로딩 */
	FORCEINLINE ElementType& operator[](const size_t Index);
	FORCEINLINE const ElementType& operator[](const size_t Index) const;
};

template <typename Number>
	requires std::is_arithmetic_v<Number>
FString FString::FromInt(Number Num)
{
#if USE_WIDECHAR
    return FString{std::to_wstring(Num)};
#else
    return FString{std::to_string(Num)};
#endif
}

inline size_t FString::FindLastOf(const ElementType* Char) const
{
	return PrivateString.find_last_of(Char);
}

FORCEINLINE int32 FString::Len() const
{
    return static_cast<int32>(PrivateString.length());
}

FORCEINLINE bool FString::IsEmpty() const
{
    return PrivateString.empty();
}

FORCEINLINE const FString::ElementType* FString::operator*() const
{
    return PrivateString.c_str();
}

FORCEINLINE FString FString::operator+(const FString& SubStr)
{
	return this->PrivateString + SubStr.PrivateString;
}

inline FString FString::operator+(const ElementType* Rhs)
{
	return this->PrivateString + Rhs;
}

FString operator+(const FString& Lhs, const FString& Rhs)
{
	FString CopyLhs{ Lhs };
	return CopyLhs += Rhs;
}

FORCEINLINE bool FString::operator==(const FString& Rhs) const
{
    return Equals(Rhs, ESearchCase::IgnoreCase);
}

FORCEINLINE bool FString::operator==(const ElementType* Rhs) const
{
    return Equals(Rhs);
}

FORCEINLINE FString& FString::operator+=(const FString& SubStr)
{
    this->PrivateString += SubStr.PrivateString;
    return *this;
}

FORCEINLINE FString& FString::operator+=(const TCHAR* Rhs)
{
	this->PrivateString += Rhs;
	return *this;
}

FORCEINLINE TCHAR& FString::operator[](const size_t Index)
{
	return PrivateString[Index];
}

FORCEINLINE const TCHAR& FString::operator[](const size_t Index) const
{
	return PrivateString[Index];
}

FORCEINLINE const char* FString::c_char() const
{
#if USE_WIDECHAR
	static std::string ConvertedStr;
	ConvertedStr = ConvertToMultibyte(PrivateString.c_str());
	return ConvertedStr.c_str();
#else
	return PrivateString.c_str();
#endif // USE_WIDECHAR
}

inline const wchar_t* FString::c_wchar() const
{
#if USE_WIDECHAR
	return PrivateString.c_str();
#else
	static std::wstring ConvertedStr;
	ConvertedStr = ConvertToWideChar(PrivateString.c_str());
	return ConvertedStr.c_str();
#endif // USE_WIDECHAR
}

FORCEINLINE FString::ElementType& FString::Front()
{
	return PrivateString.front();
}

FORCEINLINE const FString::ElementType& FString::Front() const
{
	return PrivateString.front();
}

FORCEINLINE FString::ElementType& FString::Back()
{
	return PrivateString.back();
}

FORCEINLINE const FString::ElementType& FString::Back() const
{
	return PrivateString.back();
}

FORCEINLINE void FString::PopBack()
{
	PrivateString.pop_back();
}

FORCEINLINE FString::BaseStringType::iterator FString::Begin()
{
	return PrivateString.begin();
}

FORCEINLINE FString::BaseStringType::const_iterator FString::Begin() const
{
	return PrivateString.begin();
}

FORCEINLINE const FString::ElementType* FString::GetData() const
{
	return PrivateString.data();
}

template<>
struct std::hash<FString>
{
	size_t operator()(const FString& Key) const noexcept
	{
		return hash<FString::BaseStringType>()(Key.PrivateString);
	}
};

