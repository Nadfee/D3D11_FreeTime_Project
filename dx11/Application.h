#pragma once
#include "Window.h"
#include "Graphics.h"
#include "Object.h"

#include <Mouse.h>
#include <Keyboard.h>
#include <DirectXColors.h>
#include <SimpleMath.h>

#include <memory>
#include <string>
#include <vector>

#include <math.h>

using namespace DirectX::SimpleMath;
using GraphicsPtr = std::unique_ptr<Graphics>;

struct InputTK
{
	std::unique_ptr<DirectX::Keyboard> keyboard;
	std::unique_ptr<DirectX::Mouse> mouse;
	DirectX::Keyboard::State keyboardCurrState = { };
	DirectX::Mouse::State mouseCurrState = { };
	DirectX::Keyboard::KeyboardStateTracker keyboardTracker;
	DirectX::Mouse::ButtonStateTracker mouseTracker;

	void Initialize(const HWND& hwnd)
	{
		keyboard = std::make_unique<DirectX::Keyboard>();
		mouse = std::make_unique<DirectX::Mouse>();
		mouse->SetWindow(hwnd);
		mouse->SetMode(DirectX::Mouse::MODE_RELATIVE);
	}
};

struct MenuBar
{
	HMENU mainMenu;
	std::vector<HMENU> subMenus;
};

class Application : public Window
{
public:
	Application(const HINSTANCE& hInstance,
		const wchar_t* winName = L"Default Window Name",
		const INT& clientWidth = 1280,
		const INT& clientHeight = 720,
		const DWORD& style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		const DWORD& exStyle = 0);
	~Application();	

	virtual LRESULT HandleProc(const UINT& uMsg, const WPARAM& wParam, const LPARAM& lParam) override;
	void Run();

private:

	GraphicsPtr graphics;	// DX11
	MenuBar menuBar;		// Win32 GUI
	InputTK input;			// Input (DXTK)

	std::vector<Object> objects;

	// Helper functions
	void InitializeScene();

	// Input
	void UpdateInput();
	void HandleKeyboardInput();
	void HandleMouseInput();

	// Application specific menu bars
	void InitializeMenu();
	void HandleWinGUI(const WPARAM& wParam);
	void Quit();

};

