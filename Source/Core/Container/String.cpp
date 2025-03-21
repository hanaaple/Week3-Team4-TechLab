#include "String.h"
#include <algorithm>
#include <cctype>

#include "Core/Math/MathUtility.h"


#if USE_WIDECHAR
std::wstring FString::ConvertToWideChar(const ANSICHAR* NarrowStr)
{
	const int Size = MultiByteToWideChar(CP_UTF8, 0, NarrowStr, -1, nullptr, 0);
	std::wstring Str;
	Str.resize(Size - 1);
	MultiByteToWideChar(CP_UTF8, 0, NarrowStr, -1, Str.data(), Size);
	return Str;
}
#endif

std::string FString::ConvertToMultibyte(const WIDECHAR* WideStr)
{
	const int Size = WideCharToMultiByte(CP_UTF8, 0, WideStr, -1, nullptr, 0, nullptr, nullptr);
	std::string Str;
	Str.resize(Size - 1);
	WideCharToMultiByte(CP_UTF8, 0, WideStr, -1, Str.data(), Size, nullptr, nullptr);
	return Str;
}

FString FString::SanitizeFloat(float InFloat)
{
#if USE_WIDECHAR
    return FString{std::to_wstring(InFloat)};
#else
    return FString{std::to_string(InFloat)};
#endif
}

float FString::ToFloat(const FString& InString)
{
#if USE_WIDECHAR
	return std::stof(InString.c_char());
#else
	return std::stof(InString.c_wchar());
#endif
}

void FString::Empty()
{
    PrivateString.clear();
}

bool FString::Equals(const FString& Other, ESearchCase::Type SearchCase) const
{
    const int32 Num = Len();
    const int32 OtherNum = Other.Len();

    if (Num != OtherNum)
    {
        // Handle special case where FString() == FString("")
        return Num + OtherNum == 1;
    }
    else if (Num > 1)
    {
        if (SearchCase == ESearchCase::CaseSensitive)
        {
        	return TCString<ElementType>::Strcmp(**this, *Other) == 0;
        }
        else
        {
			/** a.k.a Stricmp */
        	return std::ranges::equal(
		        PrivateString, Other.PrivateString, [](char a, char b)
	        {
		        return std::tolower(a) == std::tolower(b);
	        });
        }
    }

    return true;
}

bool FString::Contains(const FString& SubStr, ESearchCase::Type SearchCase, ESearchDir::Type SearchDir) const
{
    return Find(SubStr, SearchCase, SearchDir, 0) != INDEX_NONE;
}

int32 FString::Find(
    const FString& SubStr, ESearchCase::Type SearchCase, ESearchDir::Type SearchDir, int32 StartPosition
) const
{
    if (SubStr.IsEmpty() || IsEmpty())
    {
        return INDEX_NONE;
    }

    const ElementType* StrPtr = **this;
    const ElementType* SubStrPtr = *SubStr;
    const int32 StrLen = Len();
    const int32 SubStrLen = SubStr.Len();

    auto CompareFunc = [SearchCase](ElementType A, ElementType B) -> bool {
        return (SearchCase == ESearchCase::IgnoreCase) ? 
            tolower(A) == tolower(B) : A == B;
    };

    auto FindSubString = [&](int32 Start, int32 End, int32 Step) -> int32 {
        for (int32 i = Start; i != End; i += Step)
        {
            bool Found = true;
            for (int32 j = 0; j < SubStrLen; ++j)
            {
                if (!CompareFunc(StrPtr[i + j], SubStrPtr[j]))
                {
                    Found = false;
                    break;
                }
            }
            if (Found)
            {
                return i;
            }
        }
        return INDEX_NONE;
    };

    if (SearchDir == ESearchDir::FromStart)
    {
        StartPosition = FMath::Clamp(StartPosition, 0, StrLen - SubStrLen);
        return FindSubString(StartPosition, StrLen - SubStrLen + 1, 1);
    }
    else // ESearchDir::FromEnd
    {
        StartPosition = (StartPosition == INDEX_NONE) ? StrLen - SubStrLen : FMath::Min(StartPosition, StrLen - SubStrLen);
        return FindSubString(StartPosition, -1, -1);
    }
}

int FString::Strnicmp(const FString& Other, const size_t Count) const
{
	return TCString<ElementType>::Strnicmp(**this, *Other, Count);
}

FString FString::Substr(const size_t Pos, const size_t Count) const
{
	if (Pos > PrivateString.length())
	{
		return {};
	}
	return {PrivateString.substr(Pos, Count)};
}

void FString::RemoveAt(const size_t Pos, const size_t Count)
{
	PrivateString.erase(Pos, Count);
}

void FString::RemoveAt(BaseStringType::iterator It, const size_t Count)
{
	PrivateString.erase(It, It + Count);
}

FString FString::ToUpper() const
{
	FString UpperString = *this;
	TCString<ElementType>::Strupr(UpperString.PrivateString.data());
	return UpperString;
}
