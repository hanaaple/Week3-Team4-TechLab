#pragma once
#include <vector>

#include "Core/AbstractClass/Singleton.h"
#include "Core/Math/Vector.h"
#include "Core/Container/Map.h"
#include "Core/Container/Array.h"
#include <functional>
#include <atomic>

/**
 * 윈도우 키 코드 열거형
 * @note https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
 */
enum class EKeyCode
{
	Q = 0x51, W = 0x57, E = 0x45, R = 0x52, T = 0x54,
	Y = 0x59, U = 0x55, I = 0x49, O = 0x4F, P = 0x50,
	A = 0x41, S = 0x53, D = 0x44, F = 0x46, G = 0x47,
	H = 0x48, J = 0x4A, K = 0x4B, L = 0x4C,
	Z = 0x5A, X = 0x58, C = 0x43, V = 0x56, B = 0x42,
	N = 0x4E, M = 0x4D,

	Left = VK_LEFT, Right = VK_RIGHT, Down = VK_DOWN, Up = VK_UP,
	LButton = VK_LBUTTON, MButton = VK_MBUTTON, RButton = VK_RBUTTON,

	LeftShift = VK_LSHIFT, Leftcontrol = VK_LCONTROL, LeftAlt = VK_LMENU,
	RightShift = VK_RSHIFT, RightControl = VK_RCONTROL, RightAlt = VK_RMENU,

	End = VK_END
};

enum class EKeyState : uint8
{
	Down,
	Press,
	UP,
	None,
};

class KeyCallbackWrapper
{
public:
	KeyCallbackWrapper(std::function<void()> Callback, uint32 uuid) : Callback(Callback), ID(uuid) {}

	void operator()() { Callback(); }

	uint32 GetID() const { return ID; }
private:
	std::function<void()> Callback;
	uint32 ID;
};

class MouseCallbackWrapper
{
public:
	MouseCallbackWrapper(std::function<void(const FVector&)> Callback, uint32 uuid) : Callback(Callback), ID(uuid) {}

	void operator()(const FVector& MousePos) { Callback(MousePos); }

	uint32 GetID() const { return ID; }
private:
	std::function<void(const FVector&)> Callback;
	uint32 ID;
};

class APlayerInput : public TSingleton<APlayerInput>
{
public:
	struct Key
	{
		EKeyCode KeyCode;
		EKeyState KeyState;
		int VK_KeyCode;
		bool bPressed;
	};
    APlayerInput();

	inline bool GetKeyDown(EKeyCode key) const { return Keys[static_cast<UINT>(key)].KeyState == EKeyState::Down; }

	inline bool GetKeyPress(EKeyCode key) const { return Keys[static_cast<uint8>(key)].KeyState == EKeyState::Press; }

	inline bool GetKeyUp(EKeyCode key) const { return Keys[static_cast<uint8>(key)].KeyState == EKeyState::UP; }

	void Update(HWND hWnd, uint32 windowWidht, uint32 Height);
    
    FVector GetMousePos() { return MousePos;}
    FVector GetMouseNDCPos() { return MouseNDCPos;}
	FVector GetMouseDeltaPos() { return MouseNDCPos - MousePreNDCPos; }

	void RegisterKeyDownCallback(EKeyCode KeyCode, std::function<void()> Callback, uint32 uuid);
	void RegisterKeyPressCallback(EKeyCode KeyCode, std::function<void()> Callback, uint32 uuid);
	void RegisterKeyUpCallback(EKeyCode KeyCode, std::function<void()> Callback, uint32 uuid);

	void RegisterMouseDownCallback(EKeyCode Button, std::function<void(const FVector&)> Callback, uint32 uuid);
	void RegisterMousePressCallback(EKeyCode Button, std::function<void(const FVector&)> Callback, uint32 uuid);
	void RegisterMouseUpCallback(EKeyCode Button, std::function<void(const FVector&)> Callback, uint32 uuid);

private:
	void CreateKeys();
	void ClearKeys();

	bool IsKeyDown(EKeyCode code);

	void UpdateKey(Key& key);
	void UpdateKeyDown(Key& key);
	void UpdateKeyUp(Key& key);

	void SetMousePos(HWND hWnd, uint32 screenWeight, uint32 screenHeight);

	FVector CalNDCPos(FVector MousePos, FVector WindowSize);

private:
	// Key 이벤트에 대한 콜백들을 저장하는 맵 (각 키마다 여러 콜백을 가질 수 있음)
	TMap<EKeyCode, TArray<KeyCallbackWrapper>> KeyDownCallbacks;
	TMap<EKeyCode, TArray<KeyCallbackWrapper>> KeyPressCallbacks;
	TMap<EKeyCode, TArray<KeyCallbackWrapper>> KeyUpCallbacks;

	// 마우스 이벤트에 대한 콜백들을 저장 (button 0: 좌클릭, 1: 우클릭)
	TMap<EKeyCode, TArray<MouseCallbackWrapper>> MouseDownCallbacks;
	TMap<EKeyCode, TArray<MouseCallbackWrapper>> MousePressCallbacks;
	TMap<EKeyCode, TArray<MouseCallbackWrapper>> MouseUpCallbacks;

	TArray<Key> Keys;

    bool bIsBlockInput = false;

    FVector MousePreNDCPos;
    FVector MousePos;
    FVector MouseNDCPos;
};
