#include "SkyboxPass.h"



SkyboxPass::SkyboxPass(RendererPtr renderer) :
	renderer(renderer),
	viewMatrixBuffer(renderer->viewMatrixBuffer),
	projectionMatrixBuffer(renderer->projectionMatrixBuffer)
{
	CreateSampler();
	CreateTextureCubeFromFiles();
	LoadShaders();
	CreateRasterizerState();
	CreateDepthStencilView();

	// Cube geometry is sent as immediate buffer in shader!
}

SkyboxPass::~SkyboxPass()
{
}


void SkyboxPass::Render()
{
	auto devCon = renderer->GetDeviceContext();

	// Setup resources
	devCon->VSSetShader(skyboxVs.Get(), nullptr, 0);
	devCon->PSSetShader(skyboxPs.Get(), nullptr, 0);
	devCon->VSSetConstantBuffers(0, 1, viewMatrixBuffer.GetAddressOf());
	devCon->VSSetConstantBuffers(1, 1, projectionMatrixBuffer.GetAddressOf());
	devCon->PSSetShaderResources(0, 1, skyboxText.GetAddressOf());
	devCon->PSSetSamplers(0, 1, sampler.GetAddressOf());

	devCon->OMSetDepthStencilState(skyboxDss.Get(), 0);

	devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devCon->RSSetViewports(1, &renderer->GetDeviceManager()->GetVP());
	devCon->IASetInputLayout(nullptr);
	devCon->RSSetState(skyboxRss.Get());

	// Trigger immediate buffer
	devCon->Draw(36, 0);

	// Clear	
	devCon->RSSetState(NULL);
	devCon->OMSetDepthStencilState(NULL, 0);


}

void SkyboxPass::CreateSampler()
{
	// Create sampler for skybox
	D3D11_SAMPLER_DESC samplerDesc = { };
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; //  D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT -- If you use this an go away diagonally to a texture, you will see clearly when LOD changes! (Quality change)
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 1;									// not used for current filter
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;		// read in detail another time
	samplerDesc.BorderColor[0] = 0.f;
	samplerDesc.BorderColor[1] = 0.f;
	samplerDesc.BorderColor[2] = 0.f;
	samplerDesc.BorderColor[3] = 0.f;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;		// no upper limit to LOD selection

	HRESULT hr = renderer->GetDevice()->CreateSamplerState(&samplerDesc, this->sampler.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void SkyboxPass::CreateTextureCubeFromFiles()
{
	// Skybox setup
	std::vector<std::wstring> filePaths;

	filePaths.push_back(L"Textures/Skyboxes/skybox/right.jpg");
	filePaths.push_back(L"Textures/Skyboxes/skybox/left.jpg");
	filePaths.push_back(L"Textures/Skyboxes/skybox/top.jpg");
	filePaths.push_back(L"Textures/Skyboxes/skybox/bottom.jpg");
	filePaths.push_back(L"Textures/Skyboxes/skybox/front.jpg");
	filePaths.push_back(L"Textures/Skyboxes/skybox/back.jpg");

	//filePaths.push_back(L"Textures/Skyboxes/otherskybox/right.png");
	//filePaths.push_back(L"Textures/Skyboxes/otherskybox/left.png");
	//filePaths.push_back(L"Textures/Skyboxes/otherskybox/top.png");
	//filePaths.push_back(L"Textures/Skyboxes/otherskybox/bottom.png");
	//filePaths.push_back(L"Textures/Skyboxes/otherskybox/front.png");
	//filePaths.push_back(L"Textures/Skyboxes/otherskybox/back.png");

	//filePaths.push_back(L"Textures/Skyboxes/yokohama/posx.jpg");
	//filePaths.push_back(L"Textures/Skyboxes/yokohama/negx.jpg");
	//filePaths.push_back(L"Textures/Skyboxes/yokohama/posy.jpg");
	//filePaths.push_back(L"Textures/Skyboxes/yokohama/negy.jpg");
	//filePaths.push_back(L"Textures/Skyboxes/yokohama/posz.jpg");
	//filePaths.push_back(L"Textures/Skyboxes/yokohama/negz.jpg");

	skyboxText = renderer->CreateTextureCubeSRVFromFiles(filePaths);
}

void SkyboxPass::LoadShaders()
{
	// Setup Skybox Shaders
	renderer->LoadShaderBlob(L"SkyboxVS.hlsl", "VSMAIN", "vs_5_0", skyboxVsBlob.GetAddressOf());
	renderer->LoadShaderBlob(L"SkyboxPS.hlsl", "PSMAIN", "ps_5_0", skyboxPsBlob.GetAddressOf());

	HRESULT hr = renderer->GetDevice()->CreateVertexShader(skyboxVsBlob->GetBufferPointer(), skyboxVsBlob->GetBufferSize(), NULL, skyboxVs.GetAddressOf());
	assert(SUCCEEDED(hr));

	hr = renderer->GetDevice()->CreatePixelShader(skyboxPsBlob->GetBufferPointer(), skyboxPsBlob->GetBufferSize(), NULL, skyboxPs.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void SkyboxPass::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC rssDesc = { };
	rssDesc.FillMode = D3D11_FILL_SOLID;
	rssDesc.CullMode = D3D11_CULL_BACK;
	rssDesc.FrontCounterClockwise = TRUE;	// we are inside box for skybox (not default for d3d)
	rssDesc.DepthBias = 0;
	rssDesc.DepthBiasClamp = 0;
	rssDesc.SlopeScaledDepthBias = 0;
	rssDesc.DepthClipEnable = TRUE;
	rssDesc.ScissorEnable = FALSE;
	rssDesc.MultisampleEnable = FALSE;
	rssDesc.AntialiasedLineEnable = FALSE;

	HRESULT hr = renderer->GetDevice()->CreateRasterizerState(&rssDesc, skyboxRss.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void SkyboxPass::CreateDepthStencilView()
{

	// Setup DSS
	D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	HRESULT hr = renderer->GetDevice()->CreateDepthStencilState(&dsDesc, skyboxDss.GetAddressOf());
	assert(SUCCEEDED(hr));
}
