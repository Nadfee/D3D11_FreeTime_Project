#include "Application.h"

Application::Application(const HINSTANCE& hInstance) :
	counter(0)
{
	// Window
	win = std::make_unique<Window>(hInstance, L"Nad's Application", 1280, 720);

	// Graphics (D3D inside)
	graphics = std::make_unique<Graphics>(win->GetHWND(), 1920, 1080);

	// XTK Mouse & Keyboard (Input is a singleton! - Experiment)
	input = Input::GetInstance();

	// Player
	player = std::make_unique<Player>();

	// Setup camera
	fpc = Camera(77.f, win->GetAspectRatio());
}

Application::~Application()
{


}

void Application::Run()
{
	InitializeScene();

	// Loop
	MSG msg = { };
	int cnt = 0;
	while (!win->IsClosed())
	{
		++cnt;
		updateTimer.Start();

		counter += updateTimer.GetDeltaTime(GTimer::Duration::SECONDS);
		if (counter > 100.f)
			counter = 0.f;

		while (PeekMessageW(&msg, win->GetHWND(), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		UpdateObjects();
		UpdateInput();
		UpdateCamera();

		graphics->Render();

		updateTimer.Stop();


		std::wstring fps(L"FPS: " + std::to_wstring(static_cast<long long>(1.L / updateTimer.GetDeltaTime(GTimer::Duration::SECONDS))));
		// Arbitrarily slow down the FPS update so we can see FPS properly
		OutputDebugStringW(std::to_wstring(updateTimer.GetDeltaTime(GTimer::Duration::SECONDS)).c_str());
		OutputDebugStringW(L"\n");

		if (cnt % 30 == 0)
		{
			SetWindowTextW(win->GetHWND(), fps.c_str());

		}
	
			
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

	// DONE  : Implement Phong Shading (Point light with radius)
	// DONE  : Hook-up attenuation correctly
	// DONE  : Decide on how to identify a light that shouldn't be calculated - Black light (Color(0, 0, 0) indicates end-of-data by shader!)

	// DONE  : Update how Objects hold Meshes (It can hold multiple meshes!) - Preparation for .obj hook. An object can now have X amount of Meshes that invokes X amount of Draw calls (not super efficient!)

	// DONE  : Add Assimp Position, UV, Normal + Texture support (Sponza loadable!)

	// DONE  : Refactor Application so that Application OWNS a Window (not inherit!)
	// DONE  : Create an Input class

	// To-do : Implement Skybox (Learn Texture Cubes!)

	// Back burner:
	// To-do : Implement a material constant buffer for meshes (to enable impl. for blinn-phong spec. and no-texture default color)
	// To-do further down the line : Abstractions for DX11 resources (e.g Vertex Shader, Pixel Shader, Bindable system, etc.)

	graphics->UpdateProjectionMatrix(fpc.GetProjectionMatrix());

	std::vector<Vertex> triVerts =
	{
		{ Vector3(1.f, -0.5f, 0.f), Vector2(1.f, 0.f), Vector3(0.f, 0.f, -1.f) },	// Clockwise
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

	CreatePointLight("RedLight", Vector3(0.f, 10.f, 0.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.4f, 0.1f));
	CreatePointLight("GreenLight", Vector3(0.f, 0.f, 0.f), Vector3(0.f, 0.4f, 0.f), Vector3(0.f, 1.f, 0.1f));
	CreatePointLight("PurpleLight", Vector3(0.f, 0.f, 0.f), Vector3(0.3f, 0.f, 0.3f), Vector3(0.f, 0.7f, 0.1f));
	CreatePointLight("BlueLight", Vector3(0.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f), Vector3(0.f, 0.4f, 0.1f));

	/*auto& sponza = CreateObject("Sponza", "Objs/Sponza/", "Sponza.fbx");
	sponza.SetScaling(0.03f);*/

	CreatePointLight("Light4", Vector3(0.f, 6.f, -3.f), Vector3(1.f, 1.f, 1.f), Vector3(0.f, 0.7f, 0.1f));


}

void Application::UpdateObjects()
{
	auto cube0 = FindObject("Cube0");
	cube0.SetPosition(-2.f, sin(counter), cube0.GetPosition().z);
	cube0.SetRotation(0.f, counter * 100.f, 0.f);

	FindObject("Triangle1").SetPosition(4.f, cosf(counter), cos(counter));

	FindLight("RedLight")->SetPosition( /*17.f + 15.f * cosf(counter * 3.f)*/ 4.f, 1.f, 0.f);
	FindLight("RedLight")->SetAttenuation(0.f, 0.6f + cosf(counter * 2.f) * 0.2f, 0.f);

	FindLight("GreenLight")->SetPosition( 23.f, 5.f, 12.f + 15.f * cosf(counter * 3.f));
	FindLight("PurpleLight")->SetPosition(-12.f + 5.f * cosf(counter * 3.f), 2, 5.f * sinf(counter * 3.f));

	FindLight("BlueLight")->SetPosition( /*17.f + 15.f * cosf(counter * 3.f)*/ 4.f, 1.f, -4.f);
}

// Used to test dynamic object deletion
static int deleteInt = 1;
static int addInt = 1;
static bool lPressed = false;

void Application::RestoreDefaultScene()
{
	deleteInt = 1;
	addInt = 1;

	// Can't simultaneously delete object in Objects in range-for!
	for (auto pair : objects)
	{
		for (auto meshID : pair.second.GetMeshIDs())
		{
			graphics->RemoveMesh(meshID);
		}
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
	input->UpdateState();
	HandleKeyboardInput();
	HandleMouseInput();
}

void Application::UpdateCamera()
{
	auto dt = updateTimer.GetDeltaTime(GTimer::Duration::SECONDS);
	// Update only if mouse in relative mode
	if (input->MouseIsRelative())
	{
		fpc.Update(input->GetMouseDeltaX(), input->GetMouseDeltaY(), player->GetLeftRight(),
			player->GetForwardBack(), player->GetUpDown(), player->GetSpeed(),
			dt);
	}


	player->ResetState();
	graphics->UpdateViewMatrix(fpc.GetViewMatrix());
}

void Application::HandleKeyboardInput()
{
	using Key = DirectX::Keyboard::Keys;

	player->InputCheck();	// Update movement

	// Toggle between absolute and relative mouse mode
	if (input->IsKeyPressed(Key::T))
	{
		OutputDebugStringW(L"Toggle absolute/relative mouse\n");
		input->ToggleMouseMode();
	}

	if (input->IsKeyPressed(Key::G))
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

	if (input->IsKeyPressed(Key::O))
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
	if (input->IsKeyPressed(Key::P))
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

	if (input->IsKeyPressed(Key::H))
	{
		auto light = FindLight("RedLight");

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

	if (input->IsKeyPressed(Key::L))
	{
		lPressed = !lPressed;
		if (lPressed == true)
		{
			for (auto& pair : lights)
			{
				pair.second.light->SetUpdateState(false);
			}
		}
		else
		{
			for (auto& pair : lights)
			{
				pair.second.light->SetUpdateState(true);
			}
		}

	}

	if (input->IsKeyPressed(Key::R))
	{
		RestoreDefaultScene();
	}

}

void Application::HandleMouseInput()
{


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
	bool meshExists = false;
	for (auto& meshID : FindObject(id).GetMeshIDs())
	{
		meshExists = true;
		graphics->RemoveMesh(meshID);
	}

	bool objectRemoved = objects.erase(id);

	return meshExists && objectRemoved;
}

Object& Application::CreateObject(const std::string& id, std::vector<Vertex> verts, const std::wstring textureFilePath)
{
	std::vector<MeshPtr> meshes;
	meshes.push_back(graphics->CreateMesh(verts, textureFilePath));		// One mesh


	// Secondary temp mesh
	std::vector<Vertex> triVerts2 =
	{
		{ Vector3(1.f, 5.f, 0.f), Vector2(1.f, 0.f), Vector3(0.f, 0.f, -1.f) },
		{ Vector3(-1.f, 5.f, 0.f), Vector2(0.f, 1.f), Vector3(0.f, 0.f, -1.f) },
		{ Vector3(0.f, 7.f, 0.f), Vector2(1.f, 1.f), Vector3(0.f, 0.f, -1.f) }
	};



	Object obj(id, meshes);
	objects.insert({ obj.GetID(), obj });
	return FindObject(obj.GetID());
}

Object& Application::CreateObject(const std::string& id, const std::string& directory, const std::string& file)
{
	assimpLoader.LoadModel(directory + file);

	std::vector<MeshPtr> meshes;

	const auto& meshesVertsData = assimpLoader.GetMeshesVertexData();
	const auto& meshesTexturePath = assimpLoader.GetMeshesTextures();

	for (int i = 0; i < meshesVertsData.size(); ++i)
	{
		std::string texturePath = directory + meshesTexturePath[i];

		// string to wstring conversion
		std::wstring texturePathWs;
		texturePathWs.assign(texturePath.begin(), texturePath.end());

		meshes.push_back(graphics->CreateMesh(meshesVertsData[i], texturePathWs));
	}

	assimpLoader.Clear();

	Object obj(id, meshes);
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

