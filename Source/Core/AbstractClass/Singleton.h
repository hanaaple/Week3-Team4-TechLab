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
    static Derived* Instance;
    TSingleton() = default;
	~TSingleton() = default;

    friend Derived;

	static void Destroy();

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
	if (Instance == nullptr)
	{
		Instance = new Derived;
		atexit(Derived::Destroy);
	}
    return *Instance;
}

template <typename Derived>
void TSingleton<Derived>::Destroy()
{
	if (Instance)
	{
		delete Instance;
		Instance = nullptr;
	}
}

template <typename Derived>
Derived* TSingleton<Derived>::Instance = nullptr;
