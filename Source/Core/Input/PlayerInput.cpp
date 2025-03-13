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
	CreateKeys();

	MousePreNDCPos = FVector(0.0f, 0.0f, 0.0f);

	MousePos = FVector(0.0f, 0.0f, 0.0f);
	MouseNDCPos = FVector(0.0f, 0.0f, 0.0f);
}

void APlayerInput::UpdateKey(Key& key)
{
	if (GetFocus())
	{
		if (IsKeyDown(key.KeyCode))
		{
			UpdateKeyDown(key);
		}
		else
		{
			UpdateKeyUp(key);
		}
	}
	else
	{
		ClearKeys();
	}
}

void APlayerInput::UpdateKeyDown(Key& key)
{
	if (key.bPressed == true)
	{
		key.KeyState = EKeyState::Press;
	}
	else
	{
		key.KeyState = EKeyState::Down;
	}

	key.bPressed = true;
}

void APlayerInput::UpdateKeyUp(Key& key)
{
	if (key.bPressed == true)
	{
		key.KeyState = EKeyState::UP;
	}
	else
	{
		key.KeyState = EKeyState::None;
	}

	key.bPressed = false;
}

void APlayerInput::CreateKeys()
{
	for (int vk = 0; vk <= 0xFF; ++vk)
	{
		EKeyCode keyCode = static_cast<EKeyCode>(vk);

		Key key = {};
		key.bPressed = false;
		key.KeyState = EKeyState::None;
		key.KeyCode = keyCode;
		key.VK_KeyCode = vk;

		Keys.Add(key);
	}
}

void APlayerInput::ClearKeys()
{
	for (Key& key : Keys)
	{
		if (key.KeyState == EKeyState::Down || key.KeyState == EKeyState::Press)
		{
			key.KeyState = EKeyState::UP;

		}
		else if (key.KeyState == EKeyState::UP)
		{
			key.KeyState = EKeyState::None;
		}

		key.bPressed = false;
	}
}

bool APlayerInput::IsKeyDown(EKeyCode code)
{
	return GetAsyncKeyState(static_cast<int>(code)) & 0x8000;
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
	for (Key& key : Keys)
	{
		UpdateKey(key);
	}		
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
		if (GetKeyPress(key))
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
		if (GetKeyDown(button))
		{
			for (auto& callback : callbacks)
			{
				callback(MouseNDCPos);
			}
		}
	}

	for (auto& [button, callbacks] : MousePressCallbacks)
	{
		if (GetKeyPress(button))
		{
			for (auto& callback : callbacks)
			{
				callback(GetMouseDeltaPos());
			}
		}
	}

	for (auto& [button, callbacks] : MouseUpCallbacks)
	{
		if (GetKeyUp(button))
		{
			for (auto& callback : callbacks)
			{
				callback(MouseNDCPos);
			}
		}
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

void APlayerInput::RegisterMouseDownCallback(EKeyCode Button, std::function<void(const FVector&)> Callback, uint32 uuid)
{
	//if (Button != EKeyCode::LButton || Button != EKeyCode::MButton || Button != EKeyCode::RButton)
	//{
	//	return;
	//}

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

void APlayerInput::RegisterMousePressCallback(EKeyCode Button, std::function<void(const FVector&)> Callback, uint32 uuid)
{
	//if (Button != EKeyCode::LButton || Button != EKeyCode::MButton || Button != EKeyCode::RButton)
	//{
	//	return;
	//}

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

void APlayerInput::RegisterMouseUpCallback(EKeyCode Button, std::function<void(const FVector&)> Callback, uint32 uuid)
{
	//if (Button != EKeyCode::LButton || Button != EKeyCode::MButton || Button != EKeyCode::RButton)
	//{
	//	return;
	//}

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
