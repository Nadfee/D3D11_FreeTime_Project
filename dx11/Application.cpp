#include "Application.h"

Application::Application(const HINSTANCE& hInstance, 
	const wchar_t* winName, 
	const INT& clientWidth, 
	const INT& clientHeight, 
	const DWORD& style, 
	const DWORD& exStyle)
	
{
	// Window creation
	this->hInstance = hInstance;
	this->winName = winName;
	this->clientWidth = clientWidth;
	this->clientHeight = clientHeight;
	this->exStyle = style;
	this->style = exStyle;

	Window::InitializeWindow();

	// Graphics (D3D inside)
	graphics = std::make_unique<Graphics>(hwnd, clientWidth, clientHeight);

	// XTK Mouse & Keyboard
	input.Initialize(hwnd);



}

Application::~Application()
{


}

LRESULT Application::HandleProc(const UINT& uMsg, const WPARAM& wParam, const LPARAM& lParam)
{
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

	// Application related procedures below

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


}

void Application::Run()
{
	auto devMan = graphics->GetDeviceManager();
	auto devCon = graphics->GetDeviceContext();

	MSG msg = { };
	while (!isClosed)
	{
		UpdateInput();

		while (PeekMessageW(&msg, hwnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		// Rendering - Clear screen for now - Temporary
		devCon->ClearRenderTargetView(devMan.GetRTV().Get(), DirectX::Colors::BurlyWood);
		devCon->OMSetRenderTargets(1, devMan.GetRTV().GetAddressOf(), NULL);

		devCon->RSSetViewports(1, &devMan.GetVP());

		devMan.GetSwapChain()->Present(0, 0);

	}

}

void Application::UpdateInput()
{
	input.keyboardCurrState = input.keyboard->GetState();
	input.mouseCurrState = input.mouse->GetState();
	input.keyboardTracker.Update(input.keyboardCurrState);
	input.mouseTracker.Update(input.mouseCurrState);

	HandleKeyboardInput();
	HandleMouseInput();
}

void Application::HandleKeyboardInput()
{
	auto& kbTr = input.keyboardTracker;
	auto& kbSt = input.keyboardCurrState;
	auto& msTr = input.mouseTracker;
	auto& msSt = input.mouseCurrState;

	// Toggle between absolute and relative mouse mode
	if (kbTr.pressed.A)
	{
		OutputDebugStringW(L"Toggle absolute/relative mouse\n");

		if (msSt.positionMode == DirectX::Mouse::MODE_ABSOLUTE)
		{
			input.mouse->SetMode(DirectX::Mouse::MODE_RELATIVE);
		}
		else
		{
			input.mouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);
		}
	}

	else if (kbTr.pressed.G)
	{
		OutputDebugStringW(L"Pressed G\n");
	}
}

void Application::HandleMouseInput()
{
	auto& msTr = input.mouseTracker;
	auto& msSt = input.mouseCurrState;

	// Print delta mouse position
	if (msSt.positionMode == DirectX::Mouse::MODE_RELATIVE)
	{
		OutputDebugStringW(L"X: ");
		OutputDebugStringW(std::to_wstring(msSt.x).c_str());
		OutputDebugStringW(L"\n");

		OutputDebugStringW(L"Y: ");
		OutputDebugStringW(std::to_wstring(msSt.y).c_str());
		OutputDebugStringW(L"\n");

		OutputDebugStringW(L"\n");
	}

}
