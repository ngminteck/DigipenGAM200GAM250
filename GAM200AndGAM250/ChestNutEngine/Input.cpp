/******************************************************************************/
/*!
\file   Input.cpp

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function and stucts definition for Input

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include <iostream>
#include "Input.h"
#include "Utils.h"

InputSystem::InputSystem()
	:m_KeyState0{new BYTE[256]}, m_KeyState1{new BYTE[256]},
	m_MouseCurr{ POINT{0,0} }, m_MousePrev{ POINT{0,0} }
{
	GetCursorPos(&m_MouseCurr);
}

InputSystem::~InputSystem()
{
	//Delete KeyStates
	SafeArrayDelete(m_KeyState0);
	SafeArrayDelete(m_KeyState1);
}

bool InputSystem::Initialize(HWND* hwnd) 
{
	int i;
	// Initialize all the keys to being released and not pressed.
	for (i = 0; i < 256; i++)
	{
		m_KeyState0[i] = false;
		m_KeyState1[i] = false;
	}
	m_Hwnd = *hwnd;
	return true;
}

void InputSystem::Update( )
{
	BOOL getKeyboardResult;
	if (m_KeyStateActive)
	{
		GetKeyboardState(m_KeyState1);
		m_PrevKeyState = m_KeyState0;
		m_CurrKeyState = m_KeyState1;
	}
	else
	{
		getKeyboardResult = GetKeyboardState(m_KeyState0);
		m_PrevKeyState = m_KeyState1;
		m_CurrKeyState = m_KeyState0;
	}
	m_KeyStateActive = !m_KeyStateActive;

	if (InputCheckTriggered(VK_LBUTTON))
	{
		m_MousePrev = m_MouseCurr;
		GetCursorPos(&m_MouseCurr);
		ScreenToClient(m_Hwnd, &m_MouseCurr);
	}

}

bool InputSystem::InputCheckCurr(unsigned char key)
{
	return ((m_PrevKeyState[key] & 0xF0) != 0 && (m_CurrKeyState[key] & 0xF0) != 0);
}

bool InputSystem::InputCheckPrev(unsigned char key)
{
	return ((m_PrevKeyState[key] & 0xF0) == 0 && (m_CurrKeyState[key] & 0xF0) != 0);
}

bool InputSystem::InputCheckTriggered(unsigned char key)
{
	return ((m_PrevKeyState[key] & 0xF0) != 0 && (m_CurrKeyState[key] & 0xF0) == 0);
}

bool InputSystem::InputCheckReleased(unsigned char key)
{
	return ((m_PrevKeyState[key] & 0xF0) == 0 && (m_CurrKeyState[key] & 0xF0) == 0);
}

POINT InputSystem::getMousePosition(bool previousFrame)
{
	return previousFrame ? m_MousePrev : m_MouseCurr;
}

POINT InputSystem::getMouseMovement()
{
	POINT delta_mouseposition;

	delta_mouseposition.x = abs(m_MouseCurr.x - m_MousePrev.x);
	delta_mouseposition.y = abs(m_MouseCurr.y - m_MousePrev.y);

	return delta_mouseposition;
}


void InputSystem::MessageSend(Event * ) const
{
	//msg->PrintMessage("Input System");
}

void InputSystem::AddToScript(sol::state * luaState)
{
	luaState->new_usertype<InputSystem>("i",
		"KeyDown", &InputSystem::InputCheckCurr,
		"KeyUp", &InputSystem::InputCheckReleased,
		"KeyTrig", &InputSystem::InputCheckTriggered,
		"KeyPrev", &InputSystem::InputCheckPrev
		);
}
