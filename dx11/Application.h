#pragma once
#include "Window.h"
#include "Graphics.h"
#include "Object.h"
#include "Camera.h"
#include "AssimpLoader.h"
#include "Timer.h"
#include "Input.h"
#include "Player.h"

#include <DirectXColors.h>
#include <SimpleMath.h>

#include <memory>
#include <string>
#include <vector>

#include <math.h>

using namespace DirectX::SimpleMath;
using GraphicsPtr = std::unique_ptr<Graphics>;
using WindowPtr = std::unique_ptr<Window>;
using InputPtr = std::shared_ptr<Input>;
using PlayerPtr = std::unique_ptr<Player>;

class Application
{	
public:
	Application(const HINSTANCE& hInstance);
	~Application();	

	void Run();

private:
	WindowPtr win;
	AssimpLoader assimpLoader;
	GraphicsPtr graphics;	
	PlayerPtr player;
	InputPtr input;
	Camera fpc;

	std::unordered_map<std::string, Object> objects;
	std::unordered_map<std::string, PointLightHash> lights;

	// Timer
	GTimer::Timer updateTimer;
	double counter;

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


};

