#include "WindowContainer.h"
//
// 
// 
// 
// 
// Initialise window 
bool RenderWindow::Initialize(WindowContainer *pWindowContainer, HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	this->hInstance = hInstance;
	this->width = width;
	this->height = height;
	this->window_title = window_title;
	this->window_class = window_class;
	this->window_title_wide = StringConvertor::StringToWide(this->window_title);
	this->window_class_wide = StringConvertor::StringToWide(this->window_class);

	this->RegisterWindowClass();

	//create window
	// Extended window style - we are using the default. for other options, 
	this->handle = CreateWindowEx(0,
		this->window_class_wide.c_str(),
		this->window_title_wide.c_str(),
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, // Window style
		0, //Window x pos
		0, //Window y pos
		this->width,  // Window With
		this->height, // Window Height
		nullptr, // Handle to parent of this window, since this is first window, it has no parent window
		nullptr, // Handel to menu or child window indentifier
		this->hInstance, // Handle to instance of module to be used with this window
		pWindowContainer); // param to create window

	// check if window is create successfully and window handle is not null
	if (this->handle == NULL)
	{
		ErrorLogger::Log(GetLastError(), "CreateWindowEx Failed for window: "+ this->window_title);
		return false;
	}

	// Bring the window on screen
	ShowWindow(this->handle, SW_SHOW);
	SetForegroundWindow(this->handle);
	SetFocus(this->handle);
	//return true 
	return true;
}
//
// HandleMsgRedirect function
// 
LRESULT CALLBACK HandleMsgRedirect(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		// All other messages
		case WM_CLOSE:
		{
			DestroyWindow(hWnd);
			return 0;
		}

		default:
		{
			// retrive pointer to window class
			WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			// forward message to window class handler
			return pWindow->WindowProc(hWnd, uMsg, wParam, lParam);
		}
	}

}
// 
// 
// 
// window process function
// 
// 
// 
// 
//
LRESULT CALLBACK HandleMessageSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		/*case WM_CHAR:
		{
			unsigned char letter = static_cast<unsigned char>(wParam);
			return 0;
		}

		case WM_KEYDOWN:
		{
			unsigned char keycode = static_cast<unsigned char>(wParam);
			return 0;
		}*/

		case WM_NCCREATE:
		{
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);
			if (pWindow == nullptr)
			{
				ErrorLogger::Log("Critical Error: Pointer to window container is null during WM_NCCREATE.");
				exit(-1);
			}

			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
			
			return pWindow->WindowProc(hWnd, uMsg, wParam, lParam);

			
			
			//return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}

		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	
}
//
// 
// 
// 
// 
// register window class and create window struct
//
// 
// 
//
void RenderWindow::RegisterWindowClass()
{
	WNDCLASSEX wcex; // our window class
	// allocate the memory size
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.cbWndExtra = 0;
	wcex.cbClsExtra = 0;

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = HandleMessageSetup;
	wcex.hInstance = this->hInstance;
	wcex.hIcon = NULL;
	wcex.hIconSm = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = this->window_class_wide.c_str();
	
	//Register window class
	RegisterClassEx(&wcex);
	
}


//Process window Messages
bool RenderWindow::ProcessMessage()
{
	//Handle window messages
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG)); // Initialize the message structure
	while (PeekMessage(&msg, this->handle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// check if window was closed
	if (msg.message == WM_NULL)
	{
		if (!IsWindow(this->handle))
		{
			this->handle = NULL;
			UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
			return false;
		}
	}

	return true;

}

HWND RenderWindow::GetHWND() const
{
	return this->handle;
}

// RenderWindow Distructor
RenderWindow::~RenderWindow()
{
	if (this->handle != NULL)
	{
		UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
		DestroyWindow(handle);
	}
}