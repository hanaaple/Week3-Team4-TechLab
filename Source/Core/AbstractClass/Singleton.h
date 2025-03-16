#pragma once
#include <memory>
#include <mutex>


/**
 * Singleton 객체 템플릿
 */
template <typename Derived>
class TSingleton
{
private:
    static std::unique_ptr<Derived> Instance;
    TSingleton() = default;
	~TSingleton() = default;

    friend Derived;

public:

    // 이동 & 복사 생성자 제거
    TSingleton(const TSingleton&) = delete;
    TSingleton& operator=(const TSingleton&) = delete;
    TSingleton(TSingleton&&) = delete;
    TSingleton& operator=(TSingleton&&) = delete;

    static Derived& Get();
};

template <typename Derived>
Derived& TSingleton<Derived>::Get()
{
	static std::once_flag flag;
	std::call_once(flag, []()
	{
		Instance.reset(new Derived{});
	});
    return *Instance;
}

template <typename Derived>
std::unique_ptr<Derived> TSingleton<Derived>::Instance = nullptr;
