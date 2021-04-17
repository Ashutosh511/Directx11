#include <Windows.h>
#include "ErrorLogger.h"
#include "Engine.h"


int CALLBACK wWinMain(  _In_     HINSTANCE hInstance,                    // handle to instance of program
					    _In_opt_ HINSTANCE hPrevInstance,                // handle to prev instance not is use always zero
						_In_     LPWSTR lpCmdLine,						 // command line argument
						_In_     int nCmdShow )                          // command show argument
{

	Engine engine;
	engine.Initialize(hInstance, "My First Window", "MyWindowClass", 800, 600);
	
	while (engine.ProcessMessage() == true)
	{
		engine.Update();
		engine.RenderFrame();
	}
	
	return 0;
}