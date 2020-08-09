#include "Application.h"

Application::Application(const HINSTANCE& hInstance,
	const wchar_t* winName,
	const INT& clientWidth,
	const INT& clientHeight,
	const DWORD& style,
	const DWORD& exStyle) :
	frequency(10000000),		// get in second
	offset(0),
	deltaTime(0),
	counter(0)
{
	// Window creation
	this->hInstance = hInstance;
	this->winName = winName;
	this->clientWidth = clientWidth;
	this->clientHeight = clientHeight;
	this->exStyle = exStyle;
	this->style = style;

	Window::InitializeWindow();

	// Win32 - Re-size again but with menu considered for AdjustWindowRect
	Window::SetClientSize(true);
	SetMenu(hwnd, menuBar.mainMenu);

	// Graphics (D3D inside)
	graphics = std::make_unique<Graphics>(hwnd, clientWidth, clientHeight);

	// XTK Mouse & Keyboard
	input.Initialize(hwnd);

	// Setup camera
	fpc = Camera(70.f, (float)clientWidth / (float)clientHeight);

	

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
		DestroyWindow(hwnd);	// Immediately destroy window if X is pressed
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

	// Win32 GUI
	case WM_CREATE:
	{
		InitializeMenu();
		break;
	}
	
	case WM_COMMAND:
	{
		HandleWinGUI(wParam);
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
		return DefWindowProc(hwnd, uMsg, wParam, lParam);

	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);

}

void Application::Run()
{
	InitializeScene();

	double endTime = 0;
	double startTime = 0;

	MSG msg = { };
	while (!isClosed)
	{
		deltaTime = endTime - startTime;
		counter += deltaTime;
		if (counter > 100.f)
			counter = 0.f;
		startTime = GetSeconds();

		while (PeekMessageW(&msg, hwnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}



		// Maybe this kind of setup?
		// SceneManager.SetActiveScene(ID);	--> Should hold Scenes 
		// SceneManager.Run();
			// Objects.Render()	 --> Requires a shared ptr to a device context
				// Mesh.Render() --> Interfaces with a device context 

		// Rendering - Clear screen for now - Temporary
		//graphics->GetRenderer()->Render();
		//devCon->ClearRenderTargetView(devMan->GetRTV().Get(), DirectX::Colors::BurlyWood);
		//devCon->OMSetRenderTargets(1, devMan->GetRTV().GetAddressOf(), NULL);

		//devCon->RSSetViewports(1, &devMan->GetVP());

		//devMan->GetSwapChain()->Present(0, 0);

		UpdateObjects();
		UpdateInput();
		UpdateCamera();

		graphics->Frame();

		for (auto& obj : objects)
		{
			obj.Render();
		}

		graphics->Present();

		endTime = GetSeconds();
		
		SetWindowTextW(hwnd, std::to_wstring(1.f / deltaTime).c_str());
		//OutputDebugStringW(std::to_wstring(1.f / deltaTime).c_str());
		//OutputDebugStringW(L"\n");
	}

}

void Application::InitializeScene()
{
	// DONE  : Implement a Depth Stencil View to enable depth storage and depth testing!
	// DONE  : Implement Texture creation functionality and add onto object-mesh initialization (WICTextureLoader)

	// DONE  : Create a cube mesh (Prepare for lights)
	// DONE  : Modify input layout to also support Normals (Prepare for lights)
	
	// DONE  : Implement Set Rotation for Objects

	// To-do : Implement Light (Point light with radius)
	// To-do : Implement Phong Shading

	// To-do : Add tinyobjloader functionality and hook to CreateMesh 
	//		   (either by creating a finalized vertex vector or overriding the CreateMesh and passing a wrapper-implementation around tinyobjloader)
	// To-do : Implement a material constant buffer for meshes

	std::vector<Vertex> triVerts =
	{
		{ Vector3(1.f, -0.5f, 0.f), Vector2(1.f, 0.f), Vector3(0.f, 0.f, -1.f) },
		{ Vector3(-1.f, -0.5f, 0.f), Vector2(0.f, 1.f), Vector3(0.f, 0.f, -1.f) },
		{ Vector3(0.f, 1.f, 0.f), Vector2(1.f, 1.f), Vector3(0.f, 0.f, -1.f) }
	};

	std::vector<Vertex> quadVerts =
	{
		// front face
		// bottom left tri
		{ Vector3(1.f, -1.f, -1.f), Vector2(1.f, 1.f), Vector3(0.f, 0.f, -1.f) },
		{ Vector3(-1.f, -1.f, -1.f), Vector2(0.f, 1.f), Vector3(0.f, 0.f, -1.f) },
		{ Vector3(-1.f, 1.f, -1.f), Vector2(0.f, 0.f), Vector3(0.f, 0.f, -1.f) },
		// top right tri
		{ Vector3(-1.f, 1.f, -1.f), Vector2(0.f, 0.f), Vector3(0.f, 0.f, -1.f) },
		{ Vector3(1.f, 1.f, -1.f), Vector2(1.f, 0.f), Vector3(0.f, 0.f, -1.f) },
		{ Vector3(1.f, -1.f, -1.f), Vector2(1.f, 1.f), Vector3(0.f, 0.f, -1.f) },
	};

	std::vector<Vertex> cubeVerts =
	{
		// front face
		// bottom left tri
		{ Vector3(1.f, -1.f, -1.f), Vector2(1.f, 1.f), Vector3(0.f, 0.f, -1.f) },
		{ Vector3(-1.f, -1.f, -1.f), Vector2(0.f, 1.f), Vector3(0.f, 0.f, -1.f) },
		{ Vector3(-1.f, 1.f, -1.f), Vector2(0.f, 0.f), Vector3(0.f, 0.f, -1.f) },
		// top right tri
		{ Vector3(-1.f, 1.f, -1.f), Vector2(0.f, 0.f), Vector3(0.f, 0.f, -1.f) },
		{ Vector3(1.f, 1.f, -1.f), Vector2(1.f, 0.f), Vector3(0.f, 0.f, -1.f) },
		{ Vector3(1.f, -1.f, -1.f), Vector2(1.f, 1.f), Vector3(0.f, 0.f, -1.f) },

		// back face
		{ Vector3(-1.f, -1.f, 1.f), Vector2(1.f, 1.f), Vector3(0.f, 0.f, 1.f) },
		{ Vector3(1.f, -1.f, 1.f), Vector2(0.f, 1.f), Vector3(0.f, 0.f, 1.f) },
		{ Vector3(1.f, 1.f, 1.f), Vector2(0.f, 0.f), Vector3(0.f, 0.f, 1.f) },

		{ Vector3(1.f, 1.f, 1.f), Vector2(0.f, 0.f), Vector3(0.f, 0.f, 1.f) },
		{ Vector3(-1.f, 1.f, 1.f), Vector2(1.f, 0.f), Vector3(0.f, 0.f, 1.f) },
		{ Vector3(-1.f, -1.f, 1.f), Vector2(1.f, 1.f), Vector3(0.f, 0.f, 1.f) },

		// left face
		{ Vector3(-1.f, -1.f, -1.f), Vector2(1.f, 1.f), Vector3(-1.f, 0.f, 0.f) },
		{ Vector3(-1.f, -1.f, 1.f), Vector2(0.f, 1.f), Vector3(-1.f, 0.f, 0.f) },
		{ Vector3(-1.f, 1.f, 1.f), Vector2(0.f, 0.f), Vector3(-1.f, 0.f, 0.f) },

		{ Vector3(-1.f, 1.f, 1.f), Vector2(0.f, 0.f), Vector3(-1.f, 0.f, 0.f) },
		{ Vector3(-1.f, 1.f, -1.f), Vector2(1.f, 0.f), Vector3(-1.f, 0.f, 0.f) },
		{ Vector3(-1.f, -1.f, -1.f), Vector2(1.f, 1.f), Vector3(-1.f, 0.f, 0.f) },

		// right face
		{ Vector3(1.f, -1.f, 1.f), Vector2(1.f, 1.f), Vector3(1.f, 0.f, 0.f)},
		{ Vector3(1.f, -1.f, -1.f), Vector2(0.f, 1.f), Vector3(1.f, 0.f, 0.f)},
		{ Vector3(1.f, 1.f, -1.f), Vector2(0.f, 0.f), Vector3(1.f, 0.f, 0.f)},

		{ Vector3(1.f, 1.f, -1.f), Vector2(0.f, 0.f), Vector3(1.f, 0.f, 0.f)},
		{ Vector3(1.f, 1.f, 1.f), Vector2(1.f, 0.f), Vector3(1.f, 0.f, 0.f)},
		{ Vector3(1.f, -1.f, 1.f), Vector2(1.f, 1.f), Vector3(1.f, 0.f, 0.f)},

		// top face
		{ Vector3(1.f, 1.f, -1.f), Vector2(1.f, 1.f), Vector3(0.f, 1.f, 0.f)},
		{ Vector3(-1.f, 1.f, -1.f), Vector2(0.f, 1.f), Vector3(0.f, 1.f, 0.f)},
		{ Vector3(-1.f, 1.f, 1.f), Vector2(0.f, 0.f), Vector3(0.f, 1.f, 0.f)},

		{ Vector3(-1.f, 1.f, 1.f), Vector2(0.f, 0.f), Vector3(0.f, 1.f, 0.f)},
		{ Vector3(1.f, 1.f, 1.f), Vector2(1.f, 0.f), Vector3(0.f, 1.f, 0.f)},
		{ Vector3(1.f, 1.f, -1.f), Vector2(1.f, 1.f), Vector3(0.f, 1.f, 0.f)},

		// bottom face
		{ Vector3(1.f, -1.f, 1.f), Vector2(1.f, 1.f), Vector3(0.f, -1.f, 0.f)},
		{ Vector3(-1.f, -1.f, 1.f), Vector2(0.f, 1.f), Vector3(0.f, -1.f, 0.f)},
		{ Vector3(-1.f, -1.f, -1.f), Vector2(0.f, 0.f), Vector3(0.f, -1.f, 0.f)},

		{ Vector3(-1.f, -1.f, -1.f), Vector2(0.f, 0.f), Vector3(0.f, -1.f, 0.f)},
		{ Vector3(1.f, -1.f, -1.f), Vector2(1.f, 0.f), Vector3(0.f, -1.f, 0.f)},
		{ Vector3(1.f, -1.f, 1.f), Vector2(1.f, 1.f), Vector3(0.f, -1.f, 0.f)}
	}; 

	// Vertex Buffer done, Matrix Buffer done (no init data and updated every frame via SetPosition()), texture: to-do
	objects.push_back(
		Object(graphics->CreateMesh(triVerts, L"Textures/moss.jpg")	
		));		

	//for (int i = -2; i < 2; ++i)
	//{
	//	Object obj(graphics->CreateMesh(quadVerts, L"Textures/minecraftstonebrick.jpg"));
	//	obj.SetPosition(Vector3(i, i, i));

	//	objects.push_back(obj);
	//}

	Object obj1(graphics->CreateMesh(quadVerts, L"Textures/moss.jpg"));
	obj1.SetPosition(Vector3(0.f, 0.f, 0.f));

	objects.push_back(obj1);

	Object obj2(graphics->CreateMesh(cubeVerts, L"Textures/minecraftstonebrick.jpg"));
	obj2.SetPosition(Vector3(-7.f, 0.f, 4.f));

	objects.push_back(obj2);
}

void Application::UpdateCamera()
{
	auto& msSt = input.mouseCurrState;

	if (msSt.positionMode == DirectX::Mouse::MODE_RELATIVE)
		fpc.Update(msSt.x, msSt.y, ply.moveLeftRight, ply.moveForwardBack, ply.moveUpDown, deltaTime);

	graphics->UpdateViewMatrix(fpc.GetViewMatrix());
	graphics->UpdateProjectionMatrix(fpc.GetProjectionMatrix());
	
	ply.Reset();
}

void Application::UpdateObjects()
{
	objects[0].SetPosition(Vector3(4.f, sin(counter), cos(counter)));

	objects[2].SetPosition(Vector3(2.f, 2.f + cos(counter), 2.f));
	//objects[2].SetRotation(12.f * cos(counter) + 12.f, 0.f, 0.f);			//  counter clockwise
	//objects[2].SetRotation(0.f, 12.f * cos(counter) + 12.f, 0.f);			//	counter clockwise
	//objects[2].SetRotation(0.f, 0.f, 12.f * cos(counter) + 12.f);			//  counter clockwise

	objects[2].SetRotation(0.f, counter * 100.f, 0.f);


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

	using key = DirectX::Keyboard;

	// Toggle between absolute and relative mouse mode
	if (kbTr.IsKeyPressed(key::T))
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

	// Movement controls: WASD + QE
	if (kbSt.IsKeyDown(key::A))
	{
		ply.moveLeftRight = -1.f;
	}
	if (kbSt.IsKeyDown(key::D))
	{
		ply.moveLeftRight = 1.f;
	}
	if (kbSt.IsKeyDown(key::W))
	{
		ply.moveForwardBack = 1.f;
	}
	if (kbSt.IsKeyDown(key::S))
	{
		ply.moveForwardBack = -1.f;
	}
	if (kbSt.IsKeyDown(key::E))
	{
		ply.moveUpDown = 1.f;
	}
	if (kbSt.IsKeyDown(key::Q))
	{
		ply.moveUpDown = -1.f;
	}

}

void Application::HandleMouseInput()
{
	auto& msTr = input.mouseTracker;
	auto& msSt = input.mouseCurrState;

	// Print delta mouse position
	//if (msSt.positionMode == DirectX::Mouse::MODE_RELATIVE)
	//{
	//	OutputDebugStringW(L"X: ");
	//	OutputDebugStringW(std::to_wstring(msSt.x).c_str());
	//	OutputDebugStringW(L"\n");

	//	OutputDebugStringW(L"Y: ");
	//	OutputDebugStringW(std::to_wstring(msSt.y).c_str());
	//	OutputDebugStringW(L"\n");

	//	OutputDebugStringW(L"\n");
	//}

}

void Application::InitializeMenu()
{
	// Win32 menu
	auto& mainMenu = menuBar.mainMenu;
	auto& subMenus = menuBar.subMenus;

	mainMenu = CreateMenu();

	subMenus.push_back(CreateMenu());
	AppendMenuW(subMenus[0], MF_STRING, 0, L"Open");	// ID 0 (wParam)
	AppendMenuW(subMenus[0], MF_STRING, 1, L"Close");	// ID 1
	AppendMenuW(mainMenu, MF_POPUP, (UINT_PTR)subMenus[0], L"Options");		// Drop down options

}

void Application::HandleWinGUI(const WPARAM& wParam)
{
	switch (wParam)
	{
	case 0:
	{
		OutputDebugStringW(L"Open\n");
		MessageBeep(MB_OK);
		break;
	}
	case 1:
	{
		OutputDebugStringW(L"Close\n");
		Quit();
		break;
	}

	}
}

void Application::Quit()
{
	if (MessageBox(hwnd, L"Really quit?", L"Application", MB_OKCANCEL) == IDOK)
	{
		DestroyWindow(hwnd);
	}
}

void Application::InitTimer() {
	//frequency = 1000; // QueryPerformanceCounter default
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&offset);
}

double Application::GetSeconds() {
	uint64_t counter = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&counter);
	return (double)(counter - offset) / frequency;
}