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
	renderer->SetBackBufferRTV();

	DrawObjects();
	Present();

	// If we would like to have multiple passes (e.g deferred) then this is the place we would call various pass setups!
	// For example:
	/*
	renderer->ShadowMapDepthPass();
	DrawFromLight();
	renderer->GeometryPass();
	Draw();
	renderer->SSAOPass();
	renderer->LightPass();

	-- Post-processing between Light Pass and Final Quad Pass --

	renderer->FinalQuadPass();

	*/



}

void Graphics::Present()
{
	renderer->Present();
}

void Graphics::UpdateViewMatrix(const Matrix& mat)
{
	renderer->UpdateViewMatrix(mat);
}

void Graphics::UpdateProjectionMatrix(const Matrix& mat)
{
	renderer->UpdateProjectionMatrix(mat);
}

MeshPtr Graphics::CreateMesh(const std::vector<Vertex>& initVertexData, std::wstring textureFilePath)
{
	MeshPtr mesh = std::make_shared<Mesh>(
		renderer->CreateVertexBuffer(initVertexData),	
		sizeof(Vertex),
		initVertexData.size(),
		renderer->CreateConstantBuffer(nullptr, sizeof(Matrix), true, true),
		renderer->CreateSRVFromFileWIC(textureFilePath)	
														
			
		);

	meshManager.AddMesh(mesh);

	return mesh;
}

bool Graphics::RemoveMesh(std::size_t identifier)
{
	return meshManager.RemoveMesh(identifier);
}


void Graphics::DrawObjects()
{
	for (const auto& pair : meshManager.GetMeshes())
	{
		auto& mesh = pair.second.mesh;
		if (mesh->ShouldRender())
		{
			renderer->DrawMesh(mesh);
		}
	}
}

