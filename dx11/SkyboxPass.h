#pragma once
#include "Renderer.h"

using RendererPtr = std::shared_ptr<Renderer>;

class SkyboxPass
{
public:
	SkyboxPass(RendererPtr renderer);
	~SkyboxPass();

	void Render();

private:

	void CreateSampler();
	void CreateTextureCubeFromFiles();
	void LoadShaders();
	void CreateRasterizerState();
	void CreateDepthStencilView();

	RendererPtr renderer;

	ComPtr<ID3D11Buffer> viewMatrixBuffer;
	ComPtr<ID3D11Buffer> projectionMatrixBuffer;

	ComPtr<ID3D11ShaderResourceView> skyboxText;
	ComPtr<ID3DBlob> skyboxVsBlob;
	ComPtr<ID3DBlob> skyboxPsBlob;
	ComPtr<ID3D11VertexShader> skyboxVs;
	ComPtr<ID3D11PixelShader> skyboxPs;
	ComPtr<ID3D11RasterizerState> skyboxRss;
	ComPtr<ID3D11DepthStencilState> skyboxDss;

	ComPtr<ID3D11SamplerState> sampler;		// D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT


};

