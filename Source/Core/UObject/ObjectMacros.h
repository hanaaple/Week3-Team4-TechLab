// ReSharper disable CppClangTidyBugproneMacroParentheses
#pragma once
#include "Class.h"


#define DECLARE_CLASS(TClass, TSuperClass) \
private: \
	TClass& operator=(TClass&&); \
	TClass& operator=(const TClass&); \
	static UClass* StaticClassInfo; \
public: \
	using Super = TSuperClass; \
	using ThisClass = TClass; \
	inline static UClass* StaticClass() { return StaticClassInfo; }


#define IMPLEMENT_CLASS(TClass) \
	UClass* TClass::StaticClassInfo = UClass
