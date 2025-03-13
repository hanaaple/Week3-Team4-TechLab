#include "PlayerInput.h"

FVector GetWndWH(HWND hWnd)
{
    RECT Rect;
    int Width , Height;
    if (GetClientRect(hWnd , &Rect)) {
        Width = Rect.right - Rect.left;
        Height = Rect.bottom - Rect.top;
    }

    return FVector(Width, Height, 0.0f);
}


APlayerInput::APlayerInput()
{
	for (auto& key : KeyState)
	{
		key = EKeyState::None;
	}

    for (auto& key : MouseState)
    {
		key = EKeyState::None;
    }

	MousePreNDCPos = FVector(0.0f, 0.0f, 0.0f);

	MousePos = FVector(0.0f, 0.0f, 0.0f);
	MouseNDCPos = FVector(0.0f, 0.0f, 0.0f);
}

void APlayerInput::KeyDown(EKeyCode key)
{
	KeyState[static_cast<uint8>(key)] = EKeyState::Down;
}

void APlayerInput::KeyPress(EKeyCode key)
{
	KeyState[static_cast<uint8>(key)] = EKeyState::Pressed;
}

void APlayerInput::KeyUp(EKeyCode key)
{
	KeyState[static_cast<uint8>(key)] = EKeyState::UP;
}

void APlayerInput::MouseDown(EMouseButton button)
{
	MouseState[static_cast<uint8>(button)] = EKeyState::Down;
}

void APlayerInput::MosuePress(EMouseButton button)
{
	MouseState[static_cast<uint8>(button)] = EKeyState::Pressed;
}

void APlayerInput::MouseUp(EMouseButton button)
{
	MouseState[static_cast<uint8>(button)] = EKeyState::UP;
}

TArray<EKeyCode> APlayerInput::GetPressedKeys() 
{
	TArray<EKeyCode> ret;

    for (int i = 0; i < 256; i++) {
        if (KeyState[i] == EKeyState::Pressed) 
		{
            ret.Add(static_cast<EKeyCode>(i));
        }
    }

    return ret;
}

void APlayerInput::UpdateKeyState()
{
	for (auto& keyState : KeyState)
	{
		if (keyState == EKeyState::Down || keyState == EKeyState::Pressed)
		{
			keyState = EKeyState::Pressed;
		}
		else if (keyState == EKeyState::UP)
		{
			keyState = EKeyState::None;
		}
	}

	for (auto& mouseState : MouseState)
	{
		if (mouseState == EKeyState::Down || mouseState == EKeyState::Pressed)
		{
			mouseState = EKeyState::Pressed;
		}
		else if (mouseState == EKeyState::UP)
		{
			mouseState = EKeyState::None;
		}
	}
}

void APlayerInput::ClearKeys()
{
	for (auto& key : KeyState)
	{
		if (key == EKeyState::Down || key == EKeyState::Pressed)
		{
			key = EKeyState::UP;
		}
		else if (key == EKeyState::UP)
		{
			key = EKeyState::None;
		}
	}
	for (auto& key : MouseState)
	{
		if (key == EKeyState::Down || key == EKeyState::Pressed)
		{
			key = EKeyState::UP;
		}
		else if (key == EKeyState::UP)
		{
			key = EKeyState::None;
		}
	}
}

void APlayerInput::SetMousePos(HWND hWnd, uint32 screenWidth, uint32 screenHeight)
{
	MousePreNDCPos = MouseNDCPos;
	POINT Pts = {};
	GetCursorPos(&Pts);

	ScreenToClient(hWnd, &Pts);

	MousePos = FVector(Pts.x, Pts.y, 0);
	MouseNDCPos = CalNDCPos(MousePos, FVector(screenWidth, screenHeight, 0));
}

void APlayerInput::Update(HWND hWnd, uint32 windowWidth, uint32 windowHeight)
{
	if (GetFocus())
	{
		UpdateKeyState();
		SetMousePos(hWnd, windowWidth, windowHeight);

		for (auto& [key, callbacks] : KeyDownCallbacks)
		{
			if (GetKeyDown(key))
			{
				for (auto& callback : callbacks)
				{
					callback();
				}
			}
		}

		for (auto& [key, callbacks] : KeyPressCallbacks)
		{
			if (GetKeyPressed(key))
			{
				for (auto& callback : callbacks)
				{
					callback();
				}
			}
		}

		for (auto& [key, callbacks] : KeyUpCallbacks)
		{
			if (GetKeyUp(key))
			{
				for (auto& callback : callbacks)
				{
					callback();
				}
			}
		}

		for (auto& [button, callbacks] : MouseDownCallbacks)
		{
			if (GetMouseDown(button))
			{
				for (auto& callback : callbacks)
				{
					callback(MouseNDCPos);
				}
			}
		}

		for (auto& [button, callbacks] : MousePressCallbacks)
		{
			if (GetMousePressed(button))
			{
				for (auto& callback : callbacks)
				{
					callback(GetMouseDeltaPos());
				}
			}
		}

		for (auto& [button, callbacks] : MouseUpCallbacks)
		{
			if (GetMouseUp(button))
			{
				for (auto& callback : callbacks)
				{
					callback(MouseNDCPos);
				}
			}
		}
	}
	else
	{
		ClearKeys();
	}
}

FVector APlayerInput::CalNDCPos(FVector MousePos, FVector WindowSize)
{
    return {( MousePos.X / ( WindowSize.X / 2 ) ) - 1, ( MousePos.Y / ( WindowSize.Y / 2 ) ) - 1, 0};
}

void APlayerInput::RegisterKeyDownCallback(EKeyCode KeyCode, std::function<void()> Callback, uint32 uuid)
{
	if (KeyDownCallbacks.Contains(KeyCode))
	{
		for (auto& callbackwrap : KeyDownCallbacks[KeyCode])
		{
			if (callbackwrap.GetID() == uuid)
			{
				return;
			}
		}
	}

	KeyCallbackWrapper wrapper(Callback, uuid);
	KeyDownCallbacks[KeyCode].Add(wrapper);
}

void APlayerInput::RegisterKeyPressCallback(EKeyCode KeyCode, std::function<void()> Callback, uint32 uuid)
{
	if (KeyPressCallbacks.Contains(KeyCode))
	{
		for (auto& callback : KeyPressCallbacks[KeyCode])
		{
			if (callback.GetID() == uuid)
			{
				return;
			}
		}
	}

	KeyCallbackWrapper wrapper(Callback, uuid);
	KeyPressCallbacks[KeyCode].Add(wrapper);
}

void APlayerInput::RegisterKeyUpCallback(EKeyCode KeyCode, std::function<void()> Callback, uint32 uuid)
{
	if (KeyUpCallbacks.Contains(KeyCode))
	{
		for (auto& callback : KeyUpCallbacks[KeyCode])
		{
			if (callback.GetID() == uuid)
			{
				return;
			}
		}
	}

	KeyCallbackWrapper wrapper(Callback, uuid);
	KeyUpCallbacks[KeyCode].Add(wrapper);
}

void APlayerInput::RegisterMouseDownCallback(EMouseButton Button, std::function<void(const FVector&)> Callback, uint32 uuid)
{
	if (Button < EMouseButton::Left || EMouseButton::End < Button)
	{
		return;
	}

	if (MouseDownCallbacks.Contains(Button))
	{
		for (auto& callback : MouseDownCallbacks[Button])
		{
			if (callback.GetID() == uuid)
			{
				return;
			}
		}
	}

	MouseCallbackWrapper wrapper(Callback, uuid);
	MousePressCallbacks[Button].Add(wrapper);
}

void APlayerInput::RegisterMousePressCallback(EMouseButton Button, std::function<void(const FVector&)> Callback, uint32 uuid)
{
	if (Button < EMouseButton::Left || EMouseButton::End < Button)
	{
		return;
	}

	if (MousePressCallbacks.Contains(Button))
	{
		for (auto& callback : MousePressCallbacks[Button])
		{
			if (callback.GetID() == uuid)
			{
				return;
			}
		}
	}

	MouseCallbackWrapper wrapper(Callback, uuid);
	MousePressCallbacks[Button].Add(wrapper);
}

void APlayerInput::RegisterMouseUpCallback(EMouseButton Button, std::function<void(const FVector&)> Callback, uint32 uuid)
{
    if (Button < EMouseButton::Left ||  EMouseButton::End < Button)
    {
        return;
    }

	if (MouseUpCallbacks.Contains(Button))
	{
		for (auto& callback : MouseUpCallbacks[Button])
		{
			if (callback.GetID() == uuid)
			{
				return;
			}
		}
	}

	MouseCallbackWrapper wrapper(Callback, uuid);
	MousePressCallbacks[Button].Add(wrapper);
}
