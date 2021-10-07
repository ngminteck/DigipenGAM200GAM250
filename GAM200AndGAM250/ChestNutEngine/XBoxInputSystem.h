#pragma once
#ifndef _XBOXINPUT_H_
#define _XBOXINPUT_H_

// No MFC
#define WIN32_LEAN_AND_MEAN

// We need the Windows Header and the XInput Header

#include <windows.h>
#include <Xinput.h>
#include <iostream>
// Now, the XInput Library
// NOTE: COMMENT THIS OUT IF YOU ARE NOT USING
// A COMPILER THAT SUPPORTS THIS METHOD OF LINKING LIBRARIES
#pragma comment(lib, "XInput.lib")
#include "Input.h"
#include <sol.hpp>

enum ControllerButtons
{
	BUTTON_A = 0,
	BUTTON_B,
	BUTTON_X,
	BUTTON_Y,
	BUTTON_RT,
	BUTTON_RB,
	BUTTON_LT,
	BUTTON_LB,
	BUTTON_START,
	BUTTON_BACK,
	LSTICK,
	LSTICK_UP,
	LSTICK_DOWN,
	LSTICK_LEFT,
	LSTICK_RIGHT,
	RSTICK,
	DPAD_UP,
	DPAD_DOWN,
	DPAD_LEFT,
	DPAD_RIGHT,

	Num_Buttons
};

#define JUMPSENSITIVITY 20000

class XBoxInputSystem
{
public:
	XBoxInputSystem(InputSystem *);
	~XBoxInputSystem();
	void Update();
	bool isKeyPressed(ControllerButtons);
	bool isKeyTriggered(ControllerButtons);
	bool isKeyReleased(ControllerButtons);

	bool isLStickCurr(int);
	bool isLStickTriggered(int);
	bool isLStickReleased(int);


	bool isLTRTCurr(int);
	bool isLTRTTriggered(int);
	bool isLTRTReleased(int);
	static void AddToScript(sol::state * luaState);

private:
	class XBoxController
	{
	private:
		
	public:
		XBoxController(int playerNumber = 1);
		XINPUT_STATE GetCurrState();
		bool IsConnected();
		void Vibrate(int leftVal = 0, int rightVal = 0);
		void Update();
		int _controllerNum;
		XINPUT_STATE prev;
		XINPUT_STATE curr;
		bool isVibrating;
		bool isPressed = false;
		bool isTriggered = false;
		bool isReleased = false;
		float timeEndVibrate = 0.0f;

		int ButtonPressed[Num_Buttons];
		int ButtonTriggered[Num_Buttons];
		int ButtonReleased[Num_Buttons];

	};

	XBoxController * m_player;
	InputSystem * m_in;
};

#endif