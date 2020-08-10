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

MeshPtr Graphics::CreateMesh(std::string identifier, const std::vector<Vertex>& initVertexData, std::wstring textureFilePath)
{
	// Not done
	MeshPtr mesh = std::make_shared<Mesh>(
		renderer->CreateVertexBuffer(initVertexData),		// Vertex Buffer of mesh initialized
		sizeof(Vertex),
		initVertexData.size(),
		renderer->CreateConstantBuffer(nullptr, sizeof(Matrix), true, true),		// world matrix buffer
		renderer->CreateSRVFromFileWIC(textureFilePath),
		renderer->GetDeviceContext()						// done: pass devcon ref
															// todo: init texture
														
			
		);

	meshManager.UpdateMeshes(identifier, mesh);

	return mesh;
}


void Graphics::DrawObjects()
{
	for (const auto& mesh : meshManager.GetMeshes())
	{
		if (mesh.second.mesh->ShouldRender())
		{
			renderer->DrawMeshes(mesh.second.mesh);
		}
	}
}

/**
void Renderer::DrawMeshes(Mesh mesh)
{
	---mesh.GetWorldMatrixBuffer()...---

	devCon->VSSetConstantBuffers(0, 1, worldMatrixBuffer.GetAddressOf());
	devCon->PSSetShaderResources(0, 1, diffuseTextureSRV.GetAddressOf());
	devCon->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	devCon->Draw(vertexElementCount, 0);


}


*/

