#include "Graphics.h"

Graphics::Graphics(const HWND& hwnd, const int& clientWidth, const int& clientHeight) :
	renderer(std::make_shared<Renderer>(hwnd, clientWidth, clientHeight)),
	lightManager(renderer->CreateStructuredBuffer(nullptr, sizeof(PointLightData), 500, true, true), 500)		// Set space for 500 lights in Structured Buffer
{
	lightManager.SetBufferView(renderer->CreateBufferShaderResourceView(lightManager.GetLightsBuffer().Get(), lightManager.GetMaxLightsCount()));
	skyboxPass = std::make_unique<SkyboxPass>(renderer);
	particleSystem = std::make_unique<ParticleSystem>(renderer);

}

Graphics::~Graphics()
{

}

void Graphics::Render(double deltaTime)
{
	renderer->ClearMainRenderTarget(DirectX::Colors::Bisque);
	renderer->SetBackBufferRTV();

	// Here we can also add experimental D3D11 render passes (interface with Renderer (e.g get pass abstractions) without having to have anything on App-side)

	UpdateLightsData();
	DrawObjects();	
	skyboxPass->Render();

	
	particleSystem->Render(deltaTime);

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

PointLightPtr Graphics::CreatePointLight(const std::string& identifier, const Vector3& initPos, const Vector3& initColor, const Vector3& initAttenuation)
{
	std::shared_ptr<PointLight> light(std::make_shared<PointLight>(identifier, initPos, initColor, initAttenuation));
	lightManager.AddLight(light);
	return light;
}

bool Graphics::RemovePointLight(const std::string& identifier)
{
	return lightManager.RemoveLight(identifier);
}

void Graphics::UpdateLightsData()
{
	lightManager.UpdateLightData();
	unsigned int dataSize = lightManager.GetPointLightData().size() * sizeof(PointLightData);

	renderer->MapUpdate(lightManager.GetLightsBuffer(), (void*)lightManager.GetPointLightData().data(), dataSize, D3D11_MAP_WRITE_DISCARD);
	renderer->BindLight(1, lightManager.GetBufferView());
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

