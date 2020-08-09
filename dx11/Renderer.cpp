#include "Renderer.h"

Renderer::Renderer(const HWND& hwnd, const int& clientWidth, const int& clientHeight) :
	deviceManager(std::make_shared<DXDeviceManager>(hwnd, clientWidth, clientHeight))
{


}

Renderer::~Renderer()
{
}

void Renderer::ClearMainRenderTarget(const float* RGBA)
{
	GetDeviceContext()->ClearRenderTargetView(deviceManager->GetRTV().Get(), RGBA);
}

void Renderer::Present()
{
	GetDeviceManager()->GetSwapChain()->Present(0, 0);
}

void Renderer::UpdateMatrix(ComPtr<ID3D11Buffer> buf, const Matrix& mat)
{
	D3D11_MAPPED_SUBRESOURCE subres;
	auto devCon = deviceManager->GetDeviceContext();
	HRESULT hr = devCon->Map(buf.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subres);

	if (FAILED(hr))
		assert(false);

	memcpy(subres.pData, &mat, sizeof(Matrix));
	devCon->Unmap(buf.Get(), 0);
}

void Renderer::UpdateViewMatrix(const Matrix& mat)
{
	UpdateMatrix(viewMatrixBuffer, mat);
}

void Renderer::UpdateProjectionMatrix(const Matrix& mat)
{
	UpdateMatrix(projectionMatrixBuffer, mat);
}



ComPtr<ID3D11Buffer> Renderer::CreateVertexBuffer(const std::vector<Vertex>& initVertexData)
{
	ComPtr<ID3D11Buffer> buffer;

	D3D11_BUFFER_DESC desc = { 0 };
	desc.ByteWidth = initVertexData.size() * sizeof(Vertex);
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.pSysMem = initVertexData.data();

	auto dev = deviceManager->GetDevice();
	HRESULT hr = dev->CreateBuffer(&desc, &initData, buffer.GetAddressOf());

	if (FAILED(hr))
		assert(false);

	return buffer;

}

ComPtr<ID3D11Buffer> Renderer::CreateConstantBuffer(void* initBufferData, unsigned int bufferSize, bool cpuWrite, bool dynamic)
{
	ComPtr<ID3D11Buffer> buffer;

	D3D11_BUFFER_DESC desc = { 0 };
	desc.ByteWidth = bufferSize + (16 - (bufferSize % 16));		// 16 byte alignment
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	if (cpuWrite && dynamic) // gpu read, cpu write
	{
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if (!cpuWrite && dynamic)
	{
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
	}
	else
	{
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.CPUAccessFlags = 0;
	}

	HRESULT hr;
	auto dev = deviceManager->GetDevice();

	if (initBufferData != nullptr)
	{
		D3D11_SUBRESOURCE_DATA initData = { 0 };
		initData.pSysMem = initBufferData;
		hr = dev->CreateBuffer(&desc, &initData, buffer.GetAddressOf());
	}
	else
	{
		hr = dev->CreateBuffer(&desc, NULL, buffer.GetAddressOf());
	}

	if (FAILED(hr))
		assert(false);


	

	return buffer;
}

void Renderer::ForwardRenderSetup()
{
	// We will go with normal Draw function for now
	// Create shaders..
	// Initialize Input Layout..
	// Initialize depth stencil for depth..

	// Setup matrices..

	// Set up texture..
	// Set up sampler..

	HRESULT hr;

	auto dev = deviceManager->GetDevice();
	auto devCon = deviceManager->GetDeviceContext();
	
	// 2. Setup shaders
	LoadShaderBlob(L"VertexShader.hlsl", "VSMAIN", "vs_5_0", vsBlob.GetAddressOf());
	LoadShaderBlob(L"PixelShader.hlsl", "PSMAIN", "ps_5_0", psBlob.GetAddressOf());

	hr = dev->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, vs.GetAddressOf());
	if (FAILED(hr))
		assert(false);

	hr = dev->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, ps.GetAddressOf());
	if (FAILED(hr)) 
		assert(false);

	// 3. Setup Input Layout
	D3D11_INPUT_ELEMENT_DESC ilDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	hr = dev->CreateInputLayout(ilDesc, _countof(ilDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), il.GetAddressOf());
	if (FAILED(hr))
		assert(false);


	// Create Buffer for View and Projection Matrix
	viewMatrixBuffer = CreateConstantBuffer(nullptr, sizeof(Matrix), true, true);
	projectionMatrixBuffer = CreateConstantBuffer(nullptr, sizeof(Matrix), true, true);

	// Setting environment

	// VS
	devCon->VSSetShader(vs.Get(), NULL, NULL);
	devCon->IASetInputLayout(il.Get());


	devCon->PSSetShader(ps.Get(), NULL, NULL);

	// Misc.
	devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	devCon->OMSetRenderTargets(1, deviceManager->GetRTV().GetAddressOf(), NULL);
	devCon->RSSetViewports(1, &deviceManager->GetVP());

	// View and Projection Matrix
	devCon->VSSetConstantBuffers(1, 1, viewMatrixBuffer.GetAddressOf());
	devCon->VSSetConstantBuffers(2, 1, projectionMatrixBuffer.GetAddressOf());
}

// Shader Types: vs_5_0, ps_5_0, hs_5_0, gs_5_0, ds_5_0, cs_5_0
void Renderer::LoadShaderBlob(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR shaderType, ID3DBlob** blob)
{
	ComPtr<ID3DBlob> errorBlob;

	HRESULT hr = D3DCompileFromFile(
		fileName,
		NULL,
		NULL,
		entryPoint,
		shaderType,
		0,
		0,
		blob,
		errorBlob.GetAddressOf()
	);

	if (FAILED(hr))
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		assert(false);
	}

}

void Renderer::Render()
{
	auto devCon = deviceManager->GetDeviceContext();

	//devCon->Draw(3, 0);


}
