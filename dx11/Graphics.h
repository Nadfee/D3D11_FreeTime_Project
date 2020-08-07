#pragma once
#include "DXDeviceManager.h"

class Graphics
{
public:
	Graphics(const HWND& hwnd, const int& clientWidth, const int& clientHeight);
	~Graphics();

	// Temporary (checking if rendering works in Application)
	devicePtr GetDevice() { return this->deviceManager.GetDevice(); }
	deviceContextPtr GetDeviceContext() { return this->deviceManager.GetDeviceContext(); }
	DXDeviceManager GetDeviceManager() { return this->deviceManager; }


private:
	DXDeviceManager deviceManager;


};

