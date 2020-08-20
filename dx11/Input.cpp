#include "Input.h"

Input::Input()
{
	m_keyboard = std::make_unique<Keyboard>();
	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetWindow(GetActiveWindow());
	m_mouse->SetMode(Mouse::MODE_RELATIVE);		// Relative default!
	OutputDebugStringW(L"Input Init Success!\n");
}

Input::~Input()
{

}

std::shared_ptr<Input> Input::GetInstance()
{
	static std::shared_ptr<Input> instance(nullptr);

	if (instance == nullptr)
	{
		std::shared_ptr<Input> tmp(new Input());
		instance = tmp;
	}

	return instance;
}

void Input::UpdateState()
{
	m_keyboardCurrState = m_keyboard->GetState();
	m_mouseCurrState = m_mouse->GetState();
	m_keyboardTracker.Update(m_keyboardCurrState);
	m_mouseTracker.Update(m_mouseCurrState);
}

bool Input::IsKeyPressed(Keyboard::Keys key)
{
	if (m_keyboardTracker.IsKeyPressed(key))
	{
		return true;
	}
	return false;
}

bool Input::IsKeyDown(Keyboard::Keys key)
{
	if (m_keyboardCurrState.IsKeyDown(key))
	{
		return true;
	}
	return false;
}

void Input::ToggleMouseMode()
{
	if (m_mouseCurrState.positionMode == Mouse::MODE_ABSOLUTE)
	{
		m_mouse->SetMode(Mouse::MODE_RELATIVE);
	}
	else
	{
		m_mouse->SetMode(Mouse::MODE_ABSOLUTE);
	}
}

bool Input::MouseIsRelative()
{
	if (m_mouseCurrState.positionMode == Mouse::MODE_ABSOLUTE)
	{
		return false;
	}
	return true;
}

int Input::GetMouseDeltaX()
{
	if (m_mouseCurrState.positionMode == Mouse::MODE_RELATIVE)
	{
		return m_mouseCurrState.x;
	}
	return 0;
}

int Input::GetMouseDeltaY()
{
	if (m_mouseCurrState.positionMode == Mouse::MODE_RELATIVE)
	{
		return m_mouseCurrState.y;
	}
	return 0;
}
