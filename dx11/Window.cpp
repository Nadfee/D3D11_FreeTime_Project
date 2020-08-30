#include "Window.h"


Window::Window(const HINSTANCE& hInstance,
	const wchar_t* winName,
	const INT& clientWidth,
	const INT& clientHeight,
	const DWORD& style,
	const DWORD& exStyle) :
	isClosed(false),
	hwnd(NULL),
	hInstance(hInstance),
	winName(winName),
	clientWidth(clientWidth),
	clientHeight(clientHeight),
	style(style),
	exStyle(exStyle)
{
	InitializeWindow();

	// Win32 - Re-size again but with menu considered for AdjustWindowRect
	SetClientSize(false);
}

Window::~Window()
{
	DestroyWindow(hwnd);
	UnregisterClass(winName, hInstance);
}

void Window::InitializeWindow()
{
	// Call this in Application constructor
	CreateWin();
	SetClientSize(false);	// Setup so that we have desired client size
	ShowWindow(hwnd, SW_SHOWDEFAULT);
}

void Window::CreateWin()
{
	// init and register a window class (WNDCLASS data structure)
	// these 3 are required
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WindowProc;			// reference to the procedure to be called for this window
	wc.hInstance = hInstance;				// instance handle
	wc.lpszClassName = winName;		// class name (identifier)

	RegisterClass(&wc);

	// create window
	hwnd = CreateWindowEx(
		0,						// Default behaviour (optionals)
		winName,				// Class name (identifier)
		winName,				// Window title 
		style,					// Window style 
		CW_USEDEFAULT,			// Window x-pos
		CW_USEDEFAULT,			// Window y-pos 
		CW_USEDEFAULT,			// Default width
		CW_USEDEFAULT,			// Default height
		NULL,					// Parent window (we are top level window)
		NULL,					// Menu
		hInstance,
		(LPVOID)this			// Passing this pointer so that we can use member function procedure handling
	);


	if (hwnd == NULL)
	{
		assert(false);
	}
}


// Default WndProc
LRESULT Window::HandleProc(const UINT& uMsg, const WPARAM& wParam, const LPARAM& lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg)
	{
	case WM_CLOSE:
	{
		if (MessageBox(hwnd, L"Really quit?", L"My application", MB_OKCANCEL) == IDOK)
		{
			DestroyWindow(hwnd);
		}

		break;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);		// Puts WM_QUIT in queue and WM_QUIT causes GetMessage to return 0
		isClosed = true;
		break;
	}

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		// All painting occurs here, between BeginPaint and EndPaint.

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(hwnd, &ps);
		break;
	}

	// XTK Mouse & Keyboard
	case WM_ACTIVATEAPP:
	{
		DirectX::Keyboard::ProcessMessage(uMsg, wParam, lParam);
		DirectX::Mouse::ProcessMessage(uMsg, wParam, lParam);
		break;
	}
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		DirectX::Mouse::ProcessMessage(uMsg, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		DirectX::Keyboard::ProcessMessage(uMsg, wParam, lParam);
		break;

	case WM_SYSKEYDOWN:
		DirectX::Keyboard::ProcessMessage(uMsg, wParam, lParam);
		//if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
		//{
		//	
		//}
		break;


	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);	// default action for msg if not handled

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);	// default action for msg if not handled

}

void Window::SetTitle(const LPCWSTR& title) const
{
	SetWindowText(hwnd, title);
}

void Window::SetClientSize(bool bMenu) const
{
	RECT rect = { 0 };
	rect.right = clientWidth;
	rect.bottom = clientHeight;

	// We pass a rect that has our desired client area dimensions
	if (!AdjustWindowRectEx(&rect, style, bMenu, exStyle))
	{
		assert(false);
	}
	// Rect that pops out with information for the Window that accomodates that client area dimension

	// cx, cy are width and height! - We are just placing it at 0, 0
	if (!SetWindowPos(hwnd, HWND_TOP, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOREPOSITION))
	{
		assert(false);
	}


} 

void Window::ShowMaximized() const
{
	ShowWindow(hwnd, SW_MAXIMIZE);
}

void Window::ShowDefault() const
{
	ShowWindow(hwnd, SW_RESTORE);
}

