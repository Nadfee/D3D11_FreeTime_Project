#include "Player.h"

Player::Player() :
	m_speed(12.f),
	m_moveForwardBack(0.f),
	m_moveLeftRight(0.f),
	m_moveUpDown(0.f),
	input(Input::GetInstance())
{

}

Player::~Player()
{
}

void Player::ResetState()
{
	m_moveForwardBack = 0.f;
	m_moveLeftRight = 0.f;
	m_moveUpDown = 0.f;
}

void Player::InputCheck()
{
	using Key = DirectX::Keyboard::Keys;

	// Movement controls: WASD + QE
	if (input->IsKeyDown(Key::A))
	{
		m_moveLeftRight = -1.f;
	}
	if (input->IsKeyDown(Key::D))
	{
		m_moveLeftRight = 1.f;
	}
	if (input->IsKeyDown(Key::W))
	{
		m_moveForwardBack = 1.f;
	}
	if (input->IsKeyDown(Key::S))
	{
		m_moveForwardBack = -1.f;
	}
	if (input->IsKeyDown(Key::Space))
	{
		m_moveUpDown = 1.f;
	}
	if (input->IsKeyDown(Key::Q))
	{
		m_moveUpDown = -1.f;
	}

}

float Player::GetSpeed()
{
	return m_speed;
}

float Player::GetForwardBack()
{
	return m_moveForwardBack;
}

float Player::GetLeftRight()
{
	return m_moveLeftRight;
}

float Player::GetUpDown()
{
	return m_moveUpDown;
}
