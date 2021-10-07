/******************************************************************************/
/*!
\file   Input.h

\author Wong Zhi Jun(100%)
\par    email: w.zhijun\@digipen.edu
\par    DigiPen login: w.zhijun

\brief
This file contains function and stucts declaration for Input

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/

#pragma once
#include "InputDefines.h"
#define NOMINMAX
#include <Windows.h>
#undef NOMINMAX
#include "System.h"
#include <sol.hpp>

class InputSystem: public System
{
public:
	InputSystem();
	~InputSystem();
	
	bool Initialize(HWND *hwnd);
	void Update();

	bool InputCheckCurr(unsigned char key);
	bool InputCheckPrev(unsigned char key);
	bool InputCheckTriggered(unsigned char key);
	bool InputCheckReleased(unsigned char key);

	POINT getMousePosition(bool previousFrame = false);
	POINT getMouseMovement();

	void MessageSend(Event * msg) const;
	BYTE* m_KeyState0;
	BYTE* m_KeyState1;
	BYTE* m_CurrKeyState;
	BYTE* m_PrevKeyState;
	static void AddToScript(sol::state * luaState);
private:


	bool  m_KeyStateActive;

	POINT m_MouseCurr;
	POINT m_MousePrev;
	HWND  m_Hwnd;
};