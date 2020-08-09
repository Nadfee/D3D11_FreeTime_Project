#pragma once

#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <assert.h>
#include <wrl/client.h>

#include <BufferHelpers.h>
#include <SimpleMath.h>
#include <DirectXColors.h>
#include <WICTextureLoader.h>

using Microsoft::WRL::ComPtr;
using namespace DirectX::SimpleMath;

// C++ style type definitions
using SwapChainPtr = ComPtr<IDXGISwapChain>;
using DevicePtr = ComPtr<ID3D11Device>;
using DeviceContextPtr = ComPtr<ID3D11DeviceContext>;
using DebugPtr = ComPtr<ID3D11Debug>;
using RtvPtr = ComPtr<ID3D11RenderTargetView>;

class DXDeviceManager
{
private:
	SwapChainPtr swapChain;
	DevicePtr device;
	DeviceContextPtr deviceContext;
	DebugPtr debug;

	RtvPtr bbRTV;

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
	DeviceContextPtr GetDeviceContext() const { return deviceContext; };
	DevicePtr GetDevice() const { return device; };
	RtvPtr GetRTV() const { return bbRTV; };
	SwapChainPtr GetSwapChain() const { return swapChain; };
	DebugPtr GetDebug() const { return debug; };
	D3D11_VIEWPORT GetVP() const { return mainVP; };

};
