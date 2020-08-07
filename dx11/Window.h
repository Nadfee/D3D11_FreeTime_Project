#pragma once
#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <assert.h>

class Window
{
public:

	Window();
	virtual ~Window();

	virtual LRESULT HandleProc(const UINT& uMsg, const WPARAM& wParam, const LPARAM& lParam) = 0;

	void SetTitle(const LPCWSTR& title) const;
	void SetClientSize(const INT& width, const INT& height) const;

	const HWND GetHWND() const { return this->hwnd; };
	int GetClientWidth() const { return this->clientWidth; };
	int GetClientHeight() const { return this->clientHeight; };
	const bool IsClosed() const { return this->isClosed; };
	const float GetAspectRatio() const { return (float)clientWidth / (float)clientHeight; }

	// Maximizes the window
	void ShowMaximized() const;	
	// Restore the window to unmaximized position and size
	void ShowDefault() const;		
	


protected:	
	bool isClosed;

	HWND hwnd;						// Handle to this window
	HINSTANCE hInstance;			// Applications hInstance
	const wchar_t* winName;			// Window class name and initial title name
	INT clientWidth, clientHeight;
	DWORD style, exStyle;

	// Helper functions
	void CreateWin();
	void InitializeWindow();

	// If called outside needs to use namespace Window::
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// WindowProc needs to be a static function for class initialization! (We pass function ptr)
		// But remember that we want our action handling to happen in functions of classes (non static members)

		// Since a window has a few bytes that we can use to store data in (LongPtr) then we can use this to
		// store the ptr to the active window object of ours on class creation! (non-static)
		Window* currWin;
		if (uMsg == WM_CREATE)
		{
			// Once window is created we make sure to store the active window class in the
			// window's user data storage!
			CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			Window* currWin = reinterpret_cast<Window*>(pCreate->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)currWin);
			return 0;
			
		}
		else
		{
			currWin = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		}

		if (currWin == nullptr)
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}


		return currWin->HandleProc(uMsg, wParam, lParam);
	}


};