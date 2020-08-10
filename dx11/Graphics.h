#pragma once
#include "Renderer.h"
#include "MeshManager.h"

#include <memory>
#include <string>
#include <vector>

using RendererPtr = std::shared_ptr<Renderer>;
using MeshPtr = std::shared_ptr<Mesh>;

class Graphics
{
public:
	Graphics(const HWND& hwnd, const int& clientWidth, const int& clientHeight);
	~Graphics();

	MeshPtr CreateMesh(const std::vector<Vertex>& initVertexData, std::wstring textureFilePath);
	
	// Functions that interface with the Renderer
	void Frame();
	void Present();

	void DrawObjects();

	// Camera specific interface functions
	void UpdateViewMatrix(const Matrix& mat);
	void UpdateProjectionMatrix(const Matrix& mat);


private:
	RendererPtr renderer;

	MeshManager meshManager;

	// MeshManager (Abstract one more layer to follow single-responsibility principle?)
	// TextureManager
	// 


};

