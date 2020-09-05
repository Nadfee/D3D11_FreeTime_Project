#include "Graphics.h"

Graphics::Graphics(const HWND& hwnd, const int& clientWidth, const int& clientHeight) :
	renderer(std::make_shared<Renderer>(hwnd, clientWidth, clientHeight)),
	lightManager(renderer->CreateStructuredBuffer(nullptr, sizeof(PointLightData), 500, true, true), 500)		// Set space for 500 lights in Structured Buffer
{
	lightManager.SetBufferView(renderer->CreateBufferShaderResourceView(lightManager.GetLightsBuffer().Get(), lightManager.GetMaxLightsCount()));
	skyboxPass = std::make_unique<SkyboxPass>(renderer);
	particleSystem = std::make_unique<ParticleSystem>(renderer);


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(renderer->GetDevice().Get(), renderer->GetDeviceContext().Get());
	show_demo_window = true;



}

Graphics::~Graphics()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Graphics::Render(double deltaTime)
{
	renderer->ClearMainRenderTarget(DirectX::Colors::Bisque);
	renderer->SetBackBufferRTV();

	// Here we can also add experimental D3D11 render passes (interface with Renderer (e.g get pass abstractions) without having to have anything on App-side)

	UpdateLightsData();
	DrawObjects();	

	particleSystem->SetPosition(-15.f, 0.f, 0.f);
	particleSystem->Render(deltaTime);


	skyboxPass->Render();

	// ImGui
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	//// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	//{
	//	static float f = 0.0f;
	//	static int counter = 0;

	//	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

	//	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	//	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	//	//ImGui::Checkbox("Another Window", &show_another_window);

	//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//	//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	//	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	//		counter++;
	//	ImGui::SameLine();
	//	ImGui::Text("counter = %d", counter);

	//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//	ImGui::End();
	//}

	ImGui::ShowDemoWindow(&show_demo_window);

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());






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

void Graphics::InjectParticles()
{
	particleSystem->InjectParticles();
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

