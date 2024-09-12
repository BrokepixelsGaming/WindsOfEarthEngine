#include "Keyboard.h"

Keyboard::Keyboard()
	: m_KeyState{ 0 }
	, m_pressedKeys{ 0 }
	, codeLog{}
{
}

bool Keyboard::IsKeyDown(KeyCode keyCode) const
{
	if (keyCode < 255)
	{
		return m_KeyState[keyCode];
	}
	return false;
}

bool Keyboard::WasKeyPressed(KeyCode keyCode) const
{
	if (keyCode < 255)
	{
		return m_pressedKeys[keyCode];
	}
	return false;
}

void Keyboard::SetKeyState(KeyCode keyCode, bool isDown)
{
	if (keyCode < 255)
	{
		m_KeyState[keyCode] = isDown;
		if (isDown)
		{
			m_pressedKeys[keyCode] = isDown;
			codeLog.PushKey(keyCode);
		}
	}
}

void Keyboard::Reset()
{
	for (auto& i : m_pressedKeys)
	{
		i = false;
	}
}