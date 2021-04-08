#include <Windows.h>
#include "ErrorLogger.h"
#include "Engine.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")

int CALLBACK wWinMain(  _In_     HINSTANCE hInstance,                    // handle to instance of program
					    _In_opt_ HINSTANCE hPrevInstance,                // handle to prev instance not is use always zero
						_In_     LPWSTR lpCmdLine,						 // command line argument
						_In_     int nCmdShow )                          // command show argument
{

	Engine engine;
	engine.Initialize(hInstance, "My First Window", "MyWindowClass", 800, 600);
	
	while (engine.ProcessMessage() == true)
	{

	}
	
	return 0;
}