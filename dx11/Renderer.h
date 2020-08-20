#pragma once
#include "DXDeviceManager.h"
#include "Mesh.h"

#include <DirectXColors.h>

#include <string>
#include <assert.h>
#include <memory>
#include <array>
#include <vector>
#include <cstring>

using namespace DirectX::SimpleMath;

using DevManPtr = std::shared_ptr<DXDeviceManager>;
using MeshPtr = std::shared_ptr<Mesh>;

struct Matrices
{
	Matrix worldM;
	Matrix viewM;
	Matrix projectionM;
};

class Renderer
{
public:
	Renderer(const HWND& hwnd, const int& clientWidth, const int& clientHeight);
	~Renderer();

	DevicePtr GetDevice() { return this->deviceManager->GetDevice(); }
	DeviceContextPtr GetDeviceContext() { return this->deviceManager->GetDeviceContext(); }
	DevManPtr GetDeviceManager() { return this->deviceManager; }

	void ClearMainRenderTarget(const float* RGBA);
	void SetBackBufferRTV();
	void Present();

	void UpdateViewMatrix(const Matrix& mat);
	void UpdateProjectionMatrix(const Matrix& mat);

	void DrawMesh(const MeshPtr& mesh);
	void BindLight(unsigned int slot, ComPtr<ID3D11ShaderResourceView> srv);

	void MapUpdate(const ComPtr<ID3D11Buffer> buffer, void* data, unsigned int dataSize, D3D11_MAP mapType);
	ComPtr<ID3D11Buffer> CreateVertexBuffer(const std::vector<Vertex>& initVertexData);
	ComPtr<ID3D11Buffer> CreateConstantBuffer(void* initBufferData, unsigned int bufferSize, bool cpuWrite, bool dynamic);
	ComPtr<ID3D11Buffer> CreateStructuredBuffer(void* initBufferData, unsigned int elementSize, unsigned int elementCount, bool cpuWrite, bool dynamic);
	ComPtr<ID3D11ShaderResourceView> CreateSRVFromFileWIC(std::wstring fileName, bool mipMapOn = true);
	ComPtr<ID3D11ShaderResourceView> CreateBufferShaderResourceView(ID3D11Buffer* buffer, unsigned int elementCount);
	
	void ForwardRenderSetup();
	//void Render();

private:

	int clientWidth;
	int clientHeight;

	// DX Resources
	// Samplers..
	// RTVs..
	// SRVS..
	// etc.
	// Shaders (temp) --> Impl. class later

	// CBuffer (matrices) stored in Mesh class (impl. later)
	// SRVs stored in Mesh class
	
	DevManPtr deviceManager;

	ComPtr<ID3D11Buffer> viewMatrixBuffer;
	ComPtr<ID3D11Buffer> projectionMatrixBuffer;

	ComPtr<ID3D11Buffer> pointLightsBuffer;

	ComPtr<ID3D11DepthStencilView> dsv;
	ComPtr<ID3D11SamplerState> sampler;		// D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT


	// Shaders (Should be abstracted away in their respective classes)
	// Note Input Layout and VS go together very well
	ComPtr<ID3DBlob> vsBlob;
	ComPtr<ID3D11InputLayout> il;
	ComPtr<ID3DBlob> psBlob;
	ComPtr<ID3D11VertexShader> vs;
	ComPtr<ID3D11PixelShader> ps;


	// Helper functions
	void LoadShaderBlob(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR shaderType, ID3DBlob** blob);
	void UpdateMatrix(ComPtr<ID3D11Buffer> buf, const Matrix& mat);



	


};

