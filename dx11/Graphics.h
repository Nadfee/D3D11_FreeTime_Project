#pragma once
#include "Renderer.h"
#include "MeshManager.h"
#include "LightManager.h"

#include <memory>
#include <string>
#include <vector>

using RendererPtr = std::shared_ptr<Renderer>;
using MeshPtr = std::shared_ptr<Mesh>;
using PointLightPtr = std::shared_ptr<PointLight>;

class Graphics
{
public:
	Graphics(const HWND& hwnd, const int& clientWidth, const int& clientHeight);
	~Graphics();

	// Mesh interface functions
	MeshPtr CreateMesh(const std::vector<Vertex>& initVertexData, std::wstring textureFilePath);
	bool RemoveMesh(std::size_t identifier);

	// Experimental: Light creation interface
	PointLightPtr CreatePointLight(const std::string& identifier, const Vector3& initPos, const Vector3& initColor, const Vector3& initAttenuation);
	bool RemovePointLight(const std::string& identifier);
	
	// Functions that interface with the Renderer
	void Render();
	void Present();

	// Camera specific interface functions
	void UpdateViewMatrix(const Matrix& mat);
	void UpdateProjectionMatrix(const Matrix& mat);


private:
	void UpdateLightsData();
	void DrawObjects();

	RendererPtr renderer;

	// MeshManager (Abstract one more layer to follow single-responsibility principle?)
	// TextureManager..

	MeshManager meshManager;
	LightManager lightManager;



};

