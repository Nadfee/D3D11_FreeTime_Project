#include "Renderer.h"

Renderer::Renderer(const HWND& hwnd, const int& clientWidth, const int& clientHeight) :
	deviceManager(std::make_shared<DXDeviceManager>(hwnd, clientWidth, clientHeight)),
	clientWidth(clientWidth),
	clientHeight(clientHeight)
{
	// Make room for Lights (Structured Buffer)


}

Renderer::~Renderer()
{
	OutputDebugStringW(L"Hello\n");

}

// Note: We are also clearing depth stencil view in conjunction with render target
void Renderer::ClearMainRenderTarget(const float* RGBA)
{
	GetDeviceContext()->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH, 1.f, 0.f);
	GetDeviceContext()->ClearRenderTargetView(deviceManager->GetRTV().Get(), RGBA);
}

void Renderer::SetBackBufferRTV()
{
	GetDeviceContext()->OMSetRenderTargets(1, deviceManager->GetRTV().GetAddressOf(), dsv.Get());
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

void Renderer::DrawMesh(const MeshPtr& mesh)
{
	MapUpdate(mesh->GetWorldMatrixBuffer(), (void*)&mesh->GetWorldMatrix(), sizeof(Matrix), D3D11_MAP_WRITE_DISCARD);

	GetDeviceContext()->VSSetConstantBuffers(0, 1, mesh->GetWorldMatrixBuffer().GetAddressOf());
	GetDeviceContext()->PSSetShaderResources(0, 1, mesh->GetDiffusedTextureSRV().GetAddressOf());
	GetDeviceContext()->IASetVertexBuffers(0, 1, mesh->GetVertexBuffer().GetAddressOf(), &mesh->GetStride(), &mesh->GetOffset());
	GetDeviceContext()->Draw(mesh->GetVertexCount(), 0);

}

void Renderer::BindLight(unsigned int slot, ComPtr<ID3D11ShaderResourceView> srv)
{
	GetDeviceContext()->PSSetShaderResources(slot, 1, srv.GetAddressOf());
}



void Renderer::MapUpdate(const ComPtr<ID3D11Buffer> buffer, void* data, unsigned int dataSize, D3D11_MAP mapType)
{

	D3D11_MAPPED_SUBRESOURCE subres;
	HRESULT hr = GetDeviceContext()->Map(buffer.Get(), 0, mapType, 0, &subres);

	std::memcpy(subres.pData, data, dataSize);	

	GetDeviceContext()->Unmap(buffer.Get(), 0);
}

ComPtr<ID3D11Buffer> Renderer::CreateVertexBuffer(const std::vector<Vertex>& initVertexData)
{
	ComPtr<ID3D11Buffer> buffer;

	D3D11_BUFFER_DESC desc = { 0 };
	desc.ByteWidth = (UINT)(initVertexData.size() * sizeof(Vertex));
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

ComPtr<ID3D11Buffer> Renderer::CreateStructuredBuffer(void* initBufferData, unsigned int elementSize, unsigned int elementCount, bool cpuWrite, bool dynamic)
{
	unsigned int bufferSize = elementSize * elementCount;

	ComPtr<ID3D11Buffer> buffer;

	D3D11_BUFFER_DESC desc = { 0 };
	desc.ByteWidth = bufferSize + (elementSize - (bufferSize % elementSize));
	//desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = elementSize;

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

ComPtr<ID3D11ShaderResourceView> Renderer::CreateSRVFromFileWIC(std::wstring fileName, bool mipMapOn)
{
	ComPtr<ID3D11ShaderResourceView> srv;

	HRESULT hr;
	if (mipMapOn)
	{
		hr = DirectX::CreateWICTextureFromFile(
			deviceManager->GetDevice().Get(),
			deviceManager->GetDeviceContext().Get(),
			fileName.c_str(),
			nullptr,
			srv.GetAddressOf()
		);
	}
	else
	{
		hr = DirectX::CreateWICTextureFromFile(
			deviceManager->GetDevice().Get(),
			fileName.c_str(),
			nullptr,
			srv.GetAddressOf()
		);
	}

	assert(SUCCEEDED(hr));
	return srv;
}

ComPtr<ID3D11ShaderResourceView> Renderer::CreateBufferShaderResourceView(ID3D11Buffer* buffer, unsigned int elementCount)
{
	ComPtr<ID3D11ShaderResourceView> srv;

	D3D11_SHADER_RESOURCE_VIEW_DESC desc = { };
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	desc.Buffer.FirstElement = 0;
	desc.Buffer.NumElements = elementCount;
	//desc.Buffer.ElementWidth = structSize;
	
	HRESULT hr = GetDevice()->CreateShaderResourceView(buffer, &desc, srv.GetAddressOf());
	assert(SUCCEEDED(hr));
	return srv;
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
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		
	};

	hr = dev->CreateInputLayout(ilDesc, _countof(ilDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), il.GetAddressOf());
	if (FAILED(hr))
		assert(false);

	// Create Buffer for View and Projection Matrix
	viewMatrixBuffer = CreateConstantBuffer(nullptr, sizeof(Matrix), true, true);
	projectionMatrixBuffer = CreateConstantBuffer(nullptr, sizeof(Matrix), true, true);

	// ***** We may perhaps want to abstract these into functions later *****
	// Create depth stencil view
	ComPtr<ID3D11Texture2D> dsvText;
	D3D11_TEXTURE2D_DESC textDesc = { };
	textDesc.Width = clientWidth;
	textDesc.Height = clientHeight;
	textDesc.MipLevels = 0;
	textDesc.ArraySize = 1;
	textDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	textDesc.SampleDesc.Count = 1;
	textDesc.SampleDesc.Quality = 0;
	textDesc.Usage = D3D11_USAGE_DEFAULT;
	textDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textDesc.CPUAccessFlags = 0;
	textDesc.MiscFlags = 0;

	hr = dev->CreateTexture2D(&textDesc, NULL, dsvText.GetAddressOf());
	assert(SUCCEEDED(hr));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = { };
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = 0;		
	dsvDesc.Texture2D.MipSlice = 0;					// The index of the first mipmap level to use.

	hr = dev->CreateDepthStencilView(dsvText.Get(), &dsvDesc, dsv.GetAddressOf());
	assert(SUCCEEDED(hr));

	// Create sampler state
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

	hr = dev->CreateSamplerState(&samplerDesc, this->sampler.GetAddressOf());
	assert(SUCCEEDED(hr));

	// Setting environment

	// VS
	devCon->VSSetShader(vs.Get(), NULL, NULL);
	devCon->IASetInputLayout(il.Get());

	devCon->PSSetShader(ps.Get(), NULL, NULL);
	devCon->PSSetSamplers(0, 1, sampler.GetAddressOf());	// default wrap minmaglinear, mippoint sampler for texture

	// Misc.
	devCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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
	//auto devCon = deviceManager->GetDeviceContext();
	//ClearMainRenderTarget(DirectX::Colors::Bisque);

	//devCon->Draw(3, 0);


}
