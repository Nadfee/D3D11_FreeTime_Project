#include "Graphics.h"

Graphics::Graphics(const HWND& hwnd, const int& clientWidth, const int& clientHeight) :
	deviceManager(hwnd, clientWidth, clientHeight)
{

}

Graphics::~Graphics()
{

}