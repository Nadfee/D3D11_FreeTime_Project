#pragma once
#include "Renderer.h"
#include "Mesh.h"

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

	MeshPtr CreateMesh(const std::vector<Vertex>& initVertexData);

	// Functions that interface with the Renderer
	void Frame();
	void Present(int vsync = 0);


private:
	RendererPtr renderer;

	// MeshManager
	// TextureManager (Factories?)


};

