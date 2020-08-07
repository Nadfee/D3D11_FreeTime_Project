#pragma once

#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <assert.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

// C++ style type definitions
using swapChainPtr = ComPtr<IDXGISwapChain>;
using devicePtr = ComPtr<ID3D11Device>;
using deviceContextPtr = ComPtr<ID3D11DeviceContext>;
using debugPtr = ComPtr<ID3D11Debug>;
using rtvPtr = ComPtr<ID3D11RenderTargetView>;

class DXDeviceManager
{
private:
	swapChainPtr swapChain;
	devicePtr device;
	deviceContextPtr deviceContext;
	debugPtr debug;

	rtvPtr bbRTV;

	D3D11_VIEWPORT mainVP;

	int	clientWidth;
	int clientHeight;

	void CreateDevAndSC(const HWND& hwnd);
	void CreateBbRTV();
	void CreateMainVP();


public:
	DXDeviceManager(const HWND& hwnd, const int& clientWidth, const int& clientHeight);
	~DXDeviceManager();

	// Function is const (cannot modify 'this')
	deviceContextPtr GetDeviceContext() const { return deviceContext; };
	devicePtr GetDevice() const { return device; };
	rtvPtr GetRTV() const { return bbRTV; };
	swapChainPtr GetSwapChain() const { return swapChain; };
	debugPtr GetDebug() const { return debug; };
	D3D11_VIEWPORT GetVP() const { return mainVP; };

};
