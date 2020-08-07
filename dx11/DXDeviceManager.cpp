#include "DXDeviceManager.h"

DXDeviceManager::DXDeviceManager(const HWND& hwnd, const int& clientWidth, const int& clientHeight) :	swapChain(nullptr),
																							device(nullptr),
																							deviceContext(nullptr),
																							clientWidth(clientWidth),
																							clientHeight(clientHeight)
{
	CreateDevAndSC(hwnd);
	CreateBbRTV();
	CreateMainVP();

	// Get Debug for printing live object details at shutdown
	HRESULT hr = device.Get()->QueryInterface<ID3D11Debug>(debug.GetAddressOf());
	assert(SUCCEEDED(hr));
}

DXDeviceManager::~DXDeviceManager()
{
	debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);			// Show whats alive

	swapChain = nullptr;
	device = nullptr;
	deviceContext = nullptr;
	debug = nullptr;
}

void DXDeviceManager::CreateDevAndSC(const HWND& hwnd)
{
	DXGI_SWAP_CHAIN_DESC scDesc = { 0 };
	scDesc.BufferDesc.Width = clientWidth;
	scDesc.BufferDesc.Height = clientHeight;
	scDesc.BufferDesc.RefreshRate.Numerator = 60;	// 60/1 = 60 Hz
	scDesc.BufferDesc.RefreshRate.Denominator = 1;
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// Monitors generally do not support more than 24-bit color. (Alpha is extra)
	scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	
	// No mutlisample settings on
	scDesc.SampleDesc.Count = 1;
	scDesc.SampleDesc.Quality = 0;

	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount = 1;		// one back buffer (that we draw to)
	scDesc.OutputWindow = hwnd;
	scDesc.Windowed = TRUE;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	// discard the already show buffer after present			------------- SWITCH TO FLIP? (read more)
	scDesc.Flags = NULL;							// optional flags for swap chain behaviour

	LRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,						// Default video adapter
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,						// No software
		D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_DEBUG,		// Create flags
		NULL,	// Features targeted by this device, default: 11.0, 10.1, 10, 9.3, 9.2, 9.1 (6)
		0,
		D3D11_SDK_VERSION,
		&scDesc,
		swapChain.GetAddressOf(),
		device.GetAddressOf(),
		NULL,	// we dont need to keep track of which feature level is supported right now
		deviceContext.GetAddressOf()
	);

	assert(SUCCEEDED(hr));
}

void DXDeviceManager::CreateBbRTV()
{
	// We already have the back buffer which is contained in the swap chain!
	ID3D11Texture2D* bb;
	HRESULT hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&bb);
	assert(SUCCEEDED(hr));

	hr = device->CreateRenderTargetView(bb, NULL, bbRTV.GetAddressOf());		// NULL: create a view that accesses all of the subresources in mipmap level 0
	assert(SUCCEEDED(hr));

	bb->Release();
}

void DXDeviceManager::CreateMainVP()
{
	mainVP.TopLeftX = 0.f;
	mainVP.TopLeftY = 0.f;
	mainVP.Width = (FLOAT)clientWidth;
	mainVP.Height = (FLOAT)clientHeight;
	mainVP.MinDepth = 0.f;
	mainVP.MaxDepth = 1.f;
}