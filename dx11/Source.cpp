#include "Application.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	std::unique_ptr<Application> app = std::make_unique<Application>(hInstance);
	app->Run();

	
	return 0;
}
