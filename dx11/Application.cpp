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

		// Scenes.. Maybe this kind of setup?
		// SceneManager.SetActiveScene(ID);	--> Should hold Scenes 
		// SceneManager.Run();
			// Objects.Render()	 --> Requires a shared ptr to a device context
				// Mesh.Render() --> Interfaces with a device context 

		UpdateObjects();
		UpdateInput();
		UpdateCamera();

		graphics->Frame();

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

	// DONE  : Implement Mesh Manager (Dynamic insertion and removal of meshes implemented)

	// DONE  : Implement Light Class (Point light with radius)
	// DONE  : Implement Light Manager (For Point Lights for now)

	// To-do : Implement Phong Shading (Point light with radius)
		// To-do : Hook-up attenuation correctly
		// To-do : Decide on how to identify a light that shouldn't be calculated (e.g constant attenuation factor as something below 0)

	// To-do : Add tinyobjloader functionality and hook to CreateMesh 
	//		   (either by creating a finalized vertex vector or overriding the CreateMesh and passing a wrapper-implementation around tinyobjloader)
	// To-do : Implement a material constant buffer for meshes

	// To-do further down the line : Abstractions for DX11 resources (e.g Vertex Shader, Pixel Shader, Bindable system, etc.)

	graphics->UpdateProjectionMatrix(fpc.GetProjectionMatrix());

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

	CreateObject("Triangle1", triVerts, L"Textures/moss.jpg");

	auto& obj = CreateObject("Quad1", quadVerts, L"Textures/sand.jpg");
	obj.SetPosition(0.f, -80.f, 0.f);
	obj.SetRotation(90.f, 0.f, 0.f);
	obj.SetScaling(76.f);

	for (int i = 0; i < 10; ++i)
	{
		obj = CreateObject("Cube" + std::to_string(i), cubeVerts, L"Textures/minecraftstonebrick.jpg");
		obj.SetPosition(4.f * i, 0.f, 4.f);
	}

	FindObject("Triangle1").SetRender(false);

	// Light WORKS NOW!!!!
	CreatePointLight("Light0", Vector3(0.f, 10.f, 0.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.4f, 0.1f));
	CreatePointLight("Light1", Vector3(0.f, 0.f, 0.f), Vector3(0.f, 0.4f, 0.f), Vector3(0.f, 1.f, 0.1f));
	CreatePointLight("Light2", Vector3(0.f, 0.f, 0.f), Vector3(0.3f, 0.f, 0.3f), Vector3(0.f, 0.7f, 0.1f));
	CreatePointLight("Light3", Vector3(0.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f), Vector3(0.f, 0.4f, 0.1f));

}


void Application::UpdateObjects()
{
	auto cube0 = FindObject("Cube0");
	cube0.SetPosition(-2.f, sin(counter), cube0.GetPosition().z);
	cube0.SetRotation(0.f, counter * 100.f, 0.f);

	FindObject("Triangle1").SetPosition(4.f, cosf(counter), cos(counter));

	FindLight("Light0")->SetPosition( /*17.f + 15.f * cosf(counter * 3.f)*/ 4.f, 1.f, 0.f);
	FindLight("Light0")->SetAttenuation(0.f, 0.6f + cosf(counter * 2.f) * 0.2f, 0.f);

	FindLight("Light1")->SetPosition( 23.f, 5.f, 12.f + 15.f * cosf(counter * 3.f));
	FindLight("Light2")->SetPosition(-12.f + 5.f * cosf(counter * 3.f), 2, 5.f * sinf(counter * 3.f));

	FindLight("Light3")->SetPosition( /*17.f + 15.f * cosf(counter * 3.f)*/ 4.f, 1.f, -7.f);
}

// Used to test dynamic object deletion
static int deleteInt = 1;
static int addInt = 1;

void Application::RestoreDefaultScene()
{
	deleteInt = 1;
	addInt = 1;

	// Can't simultaneously delete object in Objects in range-for!
	for (auto pair : objects)
	{
		graphics->RemoveMesh(pair.second.GetMeshID());
	}
	objects.clear();

	for (auto pair : lights)
	{
		graphics->RemovePointLight(pair.second.light->GetID());
	}
	lights.clear();


	InitializeScene();
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

void Application::UpdateCamera()
{
	auto& msSt = input.mouseCurrState;

	// Update only if mouse in relative mode
	if (msSt.positionMode == DirectX::Mouse::MODE_RELATIVE)
		fpc.Update(msSt.x, msSt.y, ply.moveLeftRight, ply.moveForwardBack, ply.moveUpDown, ply.speed, deltaTime);

	ply.Reset();
	graphics->UpdateViewMatrix(fpc.GetViewMatrix());
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

	if (kbTr.IsKeyPressed(key::G))
	{
		// Hide/Unhide an object's render state
		auto obj = FindObject("Cube0");
		if (obj.ShouldRender() == true)
		{
			obj.SetRender(false);
		}
		else
		{
			obj.SetRender(true);

		}
	}

	if (kbTr.IsKeyPressed(key::O))
	{
		// If we try to delete an Object and it is deleted, it should crash the program.
		// Testing dynamic deletion while app is running
		RemoveObject(std::string("Cube" + std::to_string(deleteInt++)));

		// Set a breakpoint inside here to check unordered_map state
		if (deleteInt == 8)
		{
			OutputDebugStringW(L"Pressed G\n");

		}
	}
	if (kbTr.IsKeyPressed(key::P))
	{
		// Create a triangle infront of the player
		std::vector<Vertex> triVerts =
		{
			{ Vector3(1.f, -0.5f, 0.f), Vector2(1.f, 0.f), Vector3(0.f, 0.f, -1.f) },
			{ Vector3(-1.f, -0.5f, 0.f), Vector2(0.f, 1.f), Vector3(0.f, 0.f, -1.f) },
			{ Vector3(0.f, 1.f, 0.f), Vector2(1.f, 1.f), Vector3(0.f, 0.f, -1.f) }
		};

		auto& obj = CreateObject(std::string("NewTriangles" + std::to_string(addInt++)), triVerts, L"Textures/moss.jpg");

		Vector4 placement = fpc.GetPosition() + fpc.GetForward() * 7.f;
		obj.SetPosition(placement);

	}

	if (kbTr.IsKeyPressed(key::H))
	{
		auto light = FindLight("Light0");

		// Toggle light on/off
		if (light->ShouldUpdate() == true)
		{
			light->SetUpdateState(false);
		}
		else
		{
			light->SetUpdateState(true);
		}

	}

}

void Application::HandleMouseInput()
{
	auto& msTr = input.mouseTracker;
	auto& msSt = input.mouseCurrState;

	// Print delta mouse position
	if (msSt.positionMode == DirectX::Mouse::MODE_ABSOLUTE)
	{
		//OutputDebugStringW(L"X: ");
		//OutputDebugStringW(std::to_wstring(msSt.x).c_str());
		//OutputDebugStringW(L"\n");

		//OutputDebugStringW(L"Y: ");
		//OutputDebugStringW(std::to_wstring(msSt.y).c_str());
		//OutputDebugStringW(L"\n");

		//OutputDebugStringW(L"\n");
	}

}

Object& Application::FindObject(const std::string& id)
{
	// not found
	if (objects.find(id) == objects.end())
	{
		OutputDebugStringA("Object with name: ");
		OutputDebugStringA(id.c_str());
		OutputDebugStringA(" not found!\n");
		assert(false);
	}

	return objects.find(id)->second;
}

bool Application::RemoveObject(const std::string& id)
{
	bool meshRemoved = graphics->RemoveMesh(FindObject(id).GetMeshID());
	bool objectRemoved = objects.erase(id);

	return meshRemoved && objectRemoved;
}

Object& Application::CreateObject(const std::string& id, std::vector<Vertex> verts, const std::wstring textureFilePath)
{
	Object obj(id, graphics->CreateMesh(verts, textureFilePath));
	objects.insert({ obj.GetID(), obj });
	return FindObject(obj.GetID());
}

PointLightPtr Application::FindLight(const std::string& id)
{
	// not found
	if (lights.find(id) == lights.end())
	{
		OutputDebugStringA("Light with name: ");
		OutputDebugStringA(id.c_str());
		OutputDebugStringA(" not found!\n");
		assert(false);
	}

	return lights.find(id)->second.light;
}

PointLightPtr Application::CreatePointLight(const std::string& identifier, const Vector3& initPos, const Vector3& initColor, const Vector3& initAttenuation)
{
	auto light = graphics->CreatePointLight(identifier, initPos, initColor, initAttenuation);
	PointLightHash hash = { light };

	lights.insert({ identifier, hash });
	return light;
}

bool Application::RemovePointLight(const std::string& identifier)
{
	bool graphicsRemoved = graphics->RemovePointLight(identifier);
	bool appLightRemoved = lights.erase(identifier);

	return graphicsRemoved && appLightRemoved;
}

void Application::InitializeMenu()
{
	// Win32 menu
	auto& mainMenu = menuBar.mainMenu;
	auto& subMenus = menuBar.subMenus;

	mainMenu = CreateMenu();

	subMenus.push_back(CreateMenu());
	AppendMenuW(subMenus[0], MF_STRING, 0, L"Restore Scene");	// ID 0 (wParam)
	AppendMenuW(subMenus[0], MF_STRING, 1, L"Close");	// ID 1
	AppendMenuW(mainMenu, MF_POPUP, (UINT_PTR)subMenus[0], L"Options");		// Drop down options

}

void Application::HandleWinGUI(const WPARAM& wParam)
{
	switch (wParam)
	{
	case 0:
	{
		RestoreDefaultScene();
		OutputDebugStringW(L"Scene Restored\n");
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