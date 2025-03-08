﻿#pragma once
#define _TCHAR_DEFINED
#include <Windows.h>

class URenderer;

class UI
{
public:
    int currentItem = 0;
    int NumOfSpawn = 0;
    
    void Initialize(HWND hWnd, const class URenderer& Renderer);
    void Update();
    void Shutdown();

    bool bIsConsoleOpen = true;
};
