#include "Graphics.h"

Graphics::Graphics(const HWND& hwnd, const int& clientWidth, const int& clientHeight) :
	renderer(std::make_shared<Renderer>(hwnd, clientWidth, clientHeight))
{
	renderer->ForwardRenderSetup();
}

Graphics::~Graphics()
{

}

void Graphics::Frame()
{
	renderer->ClearMainRenderTarget(DirectX::Colors::Bisque);




}

void Graphics::Present(int vsync)
{
	renderer->Present(vsync);
}

MeshPtr Graphics::CreateMesh(const std::vector<Vertex>& initVertexData)
{
	// Not done
	MeshPtr mesh = std::make_shared<Mesh>(
		renderer->CreateVertexBuffer(initVertexData),		// Vertex Buffer of mesh initialized
		sizeof(Vertex),
		initVertexData.size(),
		renderer->CreateConstantBuffer(nullptr, sizeof(Matrix), true, true),		// world matrix buffer
		renderer->GetDeviceContext()						// done: pass devcon ref
															// todo: init texture
														
			
		);

	return mesh;
}

