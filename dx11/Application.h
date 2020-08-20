#pragma once
#include "Window.h"
#include "Graphics.h"
#include "Object.h"
#include "Camera.h"
#include "AssimpLoader.h"
#include "Timer.h"

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
	HMENU mainMenu = 0;
	std::vector<HMENU> subMenus;
};

struct PlayerInfo
{
	// [-1, 1]
	float moveForwardBack = 0.f;
	float moveLeftRight = 0.f;
	float moveUpDown = 0.f;
	float speed = 12.f;

	void Reset()
	{
		moveForwardBack = 0.f;
		moveLeftRight = 0.f;
		moveUpDown = 0.f;
	}

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

	AssimpLoader assimpLoader;
	GraphicsPtr graphics;	// DX11
	MenuBar menuBar;		// Win32 GUI
	InputTK input;			// Input (DXTK)
	PlayerInfo ply;

	std::unordered_map<std::string, Object> objects;
	std::unordered_map<std::string, PointLightHash> lights;

	Camera fpc;

	// Timer
	GTimer::Timer timer;
	double counter;

	// Helper functions
	void InitializeScene();
	void UpdateCamera();
	void UpdateObjects();
	void RestoreDefaultScene();

	// Object
	Object& FindObject(const std::string& id);
	bool RemoveObject(const std::string& id);
	Object& CreateObject(const std::string& id, std::vector<Vertex> verts, const std::wstring textureFilePath);
	Object& CreateObject(const std::string& id, const std::string& directory, const std::string& file);


	// Light
	PointLightPtr FindLight(const std::string& id);
	PointLightPtr CreatePointLight(const std::string& identifier, const Vector3& initPos, const Vector3& initColor, const Vector3& initRadius);
	bool RemovePointLight(const std::string& identifier);

	// Input
	void UpdateInput();
	void HandleKeyboardInput();
	void HandleMouseInput();

	// Application specific menu bars
	void InitializeMenu();
	void HandleWinGUI(const WPARAM& wParam);
	void Quit();


};

