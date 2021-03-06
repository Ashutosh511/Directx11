#pragma once
// WindowContainer Class
//
//
// Author - Ashutosh Kumar Tiwari
// Date - 07-04-2021 (covid19 lockdown)

#include "RenderWindow.h"
#include "Keyboard/KeyboardClass.h"
#include "Mouse/MouseClass.h"
#include "Graphics/Graphics.h"

class WindowContainer
{
public:
    WindowContainer();
    LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


protected:
    RenderWindow  render_window;
    KeyboardClass keyboard;
    MouseClass    mouse;
    Graphics      gfx;

private:

    
};