#pragma once
#include "Input.h"

class Player
{
public:
	Player();
	~Player();

	void ResetState();
	void InputCheck();

	float GetSpeed();
	float GetForwardBack();
	float GetLeftRight();
	float GetUpDown();

private:
	std::shared_ptr<Input> input;

	float m_speed;
	
	// States
	float m_moveForwardBack;
	float m_moveLeftRight;
	float m_moveUpDown;

};

