#pragma once
#include <memory>

#include "AbstractClass/Singleton.h"
#include "Container/Map.h"
#include "HAL/PlatformType.h"
#include "Rendering/UI.h"
#include "Rendering/URenderer.h"
#include "Slate/SSplitter.h"
#include "UObject/Casts.h"

class UObject;
class UWorld;

enum class EScreenMode : uint8
{
    Windowed,    // 창 모드
    Fullscreen,  // 전체화면 모드
    Borderless,  // 테두리 없는 창 모드
};

class UEngine : public TSingleton<UEngine>
{
public:
    // 각종 윈도우 관련 메시지(이벤트)를 처리하는 함수
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    /**
     * Application을 초기화 합니다.
     * @param hInstance 창 인스턴스
     * @param InWindowTitle 윈도우 창 이름
     * @param InWindowClassName 윈도우 클래스 이름
     * @param InScreenWidth 화면 너비
     * @param InScreenHeight 화면 높이
     * @param InScreenMode 창 모드
     * @return 초기화 여부
     */
    void Initialize(
        HINSTANCE hInstance, const WCHAR* InWindowTitle, const WCHAR* InWindowClassName, uint32 InScreenWidth,
		uint32 InScreenHeight, EScreenMode InScreenMode = EScreenMode::Windowed
    );
    void Run();

    /**
     * Application에서 사용한 자원을 정리합니다.
     */
    void Shutdown();

	URenderer* GetRenderer() const { return Renderer.get(); }
	float GetScreenRatio() const { return static_cast<float>(ScreenWidth) / static_cast<float>(ScreenHeight); }
    uint32 GetScreenWidth() const { return ScreenWidth; }
    uint32 GetScreenHeight() const { return ScreenHeight; }
    int GetInitializedScreenWidth() const { return InitializedScreenWidth; }
    int GetInitializedScreenHeight() const { return InitializedScreenHeight; }

	static float GetDeltaTime() { return UEngine::Get().EngineDeltaTime; }

private:
    void InitWindow(int InScreenWidth, int InScreenHeight);
    //void InitDevice();
    void InitRenderer();
    void InitWorld();
    void ShutdownWindow();
    void UpdateWindowSize(uint32 InScreenWidth, uint32 InScreenHeight);

	void RenderSplitScreen();

public:
	UWorld* GetWorld() const { return World; }

    HWND GetWindowHandle() const { return WindowHandle; }

    template <typename ObjectType>
        requires std::derived_from<ObjectType, UObject>
    ObjectType* GetObjectByUUID(uint32 InUUID) const;
    UObject* GetObjectByUUID(uint32 InUUID) const;


private:
    bool IsRunning = false;
    EScreenMode ScreenMode = EScreenMode::Windowed;

    const WCHAR* WindowTitle = nullptr;
    const WCHAR* WindowClassName = nullptr;
    HWND WindowHandle = nullptr;
    HINSTANCE WindowInstance = nullptr;

    int InitializedScreenWidth = 0;
    int InitializedScreenHeight = 0;

    uint32 ScreenWidth = 0;
    uint32 ScreenHeight = 0;

	float EngineDeltaTime = 0.0f;

private:
	std::unique_ptr<URenderer> Renderer;

private:
	UI ui;

private:
    UWorld* World = nullptr;

public:
    // TArray<std::shared_ptr<UObject>> GObjects;
    TMap<uint32, std::shared_ptr<UObject>> GObjects;
};

template <typename ObjectType>
	requires std::derived_from<ObjectType, UObject>
ObjectType* UEngine::GetObjectByUUID(uint32 InUUID) const
{
    if (const std::shared_ptr<UObject>* Obj = GObjects.Find(InUUID))
    {
    	if (ObjectType* CastedObject = Cast<ObjectType>(Obj->get()))
        {
            return CastedObject;
        }
    }
    return nullptr;
}
