#pragma once
#include "DXDeviceManager.h"

#include <DirectXColors.h>

#include <assert.h>
#include <memory>
#include <array>
#include <vector>

using namespace DirectX::SimpleMath;

using DevManPtr = std::shared_ptr<DXDeviceManager>;

struct Matrices
{
	Matrix worldM;
	Matrix viewM;
	Matrix projectionM;
};

struct Vertex
{
	Vector3 position;
	Vector2 uv;
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
	void Present();

	void UpdateViewMatrix(const Matrix& mat);
	void UpdateProjectionMatrix(const Matrix& mat);


	ComPtr<ID3D11Buffer> CreateVertexBuffer(const std::vector<Vertex>& initVertexData);
	ComPtr<ID3D11Buffer> CreateConstantBuffer(void* initBufferData, unsigned int bufferSize, bool cpuWrite, bool dynamic);

	// Let's start
	void ForwardRenderSetup();
	void Render();

private:

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


	// Shaders
	ComPtr<ID3DBlob> vsBlob;
	ComPtr<ID3DBlob> psBlob;
	ComPtr<ID3D11VertexShader> vs;
	ComPtr<ID3D11PixelShader> ps;

	ComPtr<ID3D11Buffer> vBuf;
	ComPtr<ID3D11Buffer> cBuf;

	ComPtr<ID3D11InputLayout> il;

	// Helper functions
	void LoadShaderBlob(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR shaderType, ID3DBlob** blob);
	void UpdateMatrix(ComPtr<ID3D11Buffer> buf, const Matrix& mat);



	


};

