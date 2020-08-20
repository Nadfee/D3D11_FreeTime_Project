#pragma once

// Include order matters!

#include "Window.h"

#include <Mouse.h>
#include <Keyboard.h>

#include <memory>

using namespace DirectX;

class Input
{
public:
	~Input();

	static std::shared_ptr<Input> GetInstance();

	Input& operator=(const Input&) = delete;

private:

	Input();

	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Mouse> m_mouse;
	Keyboard::State m_keyboardCurrState;
	Mouse::State m_mouseCurrState;
	Keyboard::KeyboardStateTracker m_keyboardTracker;
	Mouse::ButtonStateTracker m_mouseTracker;
	HWND hwnd;

public:
	void UpdateState();
	
	// Keyboard
	// Takes one action on press
	bool IsKeyPressed(Keyboard::Keys key);
	// Takes continous action if held down
	bool IsKeyDown(Keyboard::Keys key);

	// Mouse
	// Toggle mouse mode RELATIVE/ABSOLUTE
	void ToggleMouseMode();
	bool MouseIsRelative();

	// Returns 0 if mouse is in ABSOLUTE mode
	int GetMouseDeltaX();
	// Returns 0 if mouse is in ABSOLUTE mode
	int GetMouseDeltaY();

	




};



