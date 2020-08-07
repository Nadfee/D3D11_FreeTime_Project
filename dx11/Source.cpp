#include "Application.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	std::unique_ptr<Application> app = std::make_unique<Application>(hInstance, L"Nad's Application", 1280, 720);
	app->Run();

	
	return 0;
}
