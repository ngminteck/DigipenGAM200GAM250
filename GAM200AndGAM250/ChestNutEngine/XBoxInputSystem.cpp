#include "XBoxInputSystem.h"

XBoxInputSystem::XBoxInputSystem(InputSystem * in)
	:m_player{ new XBoxController{} }, m_in{ in }
{
	if (m_player->IsConnected())
		std::cout << "Player " << m_player->_controllerNum << " detected." << std::endl;
	else
		std::cout << "No controller detected" << std::endl;

}

XBoxInputSystem::~XBoxInputSystem()
{
	delete m_player;
	m_player = nullptr;
}

XBoxInputSystem::XBoxController::XBoxController(int playerNumber)
{
	// Set the Controller Number
	_controllerNum = playerNumber - 1;
}

XINPUT_STATE XBoxInputSystem::XBoxController::GetCurrState()
{
	// Zeroise the state
	ZeroMemory(&curr, sizeof(XINPUT_STATE));

	// Get the state
	XInputGetState(_controllerNum, &curr);

	return curr;
}

bool XBoxInputSystem::XBoxController::IsConnected()
{
	// Zeroise the state
	ZeroMemory(&curr, sizeof(XINPUT_STATE));

	// Get the state
	DWORD Result = XInputGetState(_controllerNum, &curr);

	if (Result == ERROR_SUCCESS)
		return true;
	else
		return false;
}

void XBoxInputSystem::XBoxController::Vibrate(int leftVal, int rightVal)
{
	// Create a Vibraton State
	XINPUT_VIBRATION Vibration;

	// Zeroise the Vibration
	ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

	// Set the Vibration Values
	Vibration.wLeftMotorSpeed = (WORD)leftVal;
	Vibration.wRightMotorSpeed = (WORD)rightVal;

	// Vibrate the controller
	XInputSetState(_controllerNum, &Vibration);
}

void XBoxInputSystem::XBoxController::Update()
{
	// Update Vibrations
	if (isVibrating)
	{
		if (timeEndVibrate > 0.0f)
		{
			Vibrate(65535, 65535);
			timeEndVibrate -= 1 / 60;
		}
		else
			Vibrate();
	}
	else
		Vibrate();

	//=========================
	//   Update Button Lists
	//=========================
	ButtonTriggered[BUTTON_A] = !ButtonPressed[BUTTON_A] && GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_A;
	ButtonTriggered[BUTTON_B] = !ButtonPressed[BUTTON_B] && GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_B;
	ButtonTriggered[BUTTON_X] = !ButtonPressed[BUTTON_X] && GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_X;
	ButtonTriggered[BUTTON_Y] = !ButtonPressed[BUTTON_Y] && GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_Y;
	ButtonTriggered[BUTTON_RT] = !ButtonPressed[BUTTON_RT] && GetCurrState().Gamepad.bRightTrigger;
	ButtonTriggered[BUTTON_RB] = !ButtonPressed[BUTTON_RB] && GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
	ButtonTriggered[BUTTON_LT] = !ButtonPressed[BUTTON_LT] && GetCurrState().Gamepad.bLeftTrigger;
	ButtonTriggered[BUTTON_LB] = !ButtonPressed[BUTTON_LB] && GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
	ButtonTriggered[BUTTON_START] = !ButtonPressed[BUTTON_START] && GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_START;
	ButtonTriggered[BUTTON_BACK] = !ButtonPressed[BUTTON_BACK] && GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK;
	ButtonTriggered[LSTICK_UP] = !ButtonPressed[LSTICK_UP] && GetCurrState().Gamepad.sThumbLY > JUMPSENSITIVITY;  //XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	ButtonTriggered[LSTICK_DOWN] = !ButtonPressed[LSTICK_DOWN] && GetCurrState().Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	ButtonTriggered[LSTICK_LEFT] = !ButtonPressed[LSTICK_LEFT] && GetCurrState().Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	ButtonTriggered[LSTICK_RIGHT] = !ButtonPressed[LSTICK_RIGHT] && GetCurrState().Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	ButtonTriggered[LSTICK] = !ButtonPressed[LSTICK] && GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
	ButtonTriggered[RSTICK] = !ButtonPressed[RSTICK] && GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
	ButtonTriggered[DPAD_UP] = !ButtonPressed[DPAD_UP] && GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
	ButtonTriggered[DPAD_DOWN] = !ButtonPressed[DPAD_DOWN] && GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
	ButtonTriggered[DPAD_LEFT] = !ButtonPressed[DPAD_LEFT] && GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
	ButtonTriggered[DPAD_RIGHT] = !ButtonPressed[DPAD_RIGHT] && GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;

	ButtonReleased[BUTTON_A] = ButtonPressed[BUTTON_A] && !(GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_A);
	ButtonReleased[BUTTON_B] = ButtonPressed[BUTTON_B] && !(GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_B);
	ButtonReleased[BUTTON_X] = ButtonPressed[BUTTON_X] && !(GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_X);
	ButtonReleased[BUTTON_Y] = ButtonPressed[BUTTON_Y] && !(GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_Y);
	ButtonReleased[BUTTON_RT] = ButtonPressed[BUTTON_RT] && !(GetCurrState().Gamepad.bRightTrigger);
	ButtonReleased[BUTTON_RB] = ButtonPressed[BUTTON_RB] && !(GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
	ButtonReleased[BUTTON_LT] = ButtonPressed[BUTTON_LT] && !(GetCurrState().Gamepad.bLeftTrigger);
	ButtonReleased[BUTTON_LB] = ButtonPressed[BUTTON_LB] && !(GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
	ButtonReleased[BUTTON_START] = ButtonPressed[BUTTON_START] && !(GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_START);
	ButtonReleased[BUTTON_BACK] = ButtonPressed[BUTTON_BACK] && !(GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK);
	ButtonReleased[LSTICK_UP] = ButtonPressed[LSTICK_UP] && !(GetCurrState().Gamepad.sThumbLY > JUMPSENSITIVITY); //XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	ButtonReleased[LSTICK_DOWN] = ButtonPressed[LSTICK_DOWN] && !(GetCurrState().Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	ButtonReleased[LSTICK_LEFT] = ButtonPressed[LSTICK_LEFT] && !(GetCurrState().Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	ButtonReleased[LSTICK_RIGHT] = ButtonPressed[LSTICK_RIGHT] && !(GetCurrState().Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	ButtonReleased[LSTICK] = ButtonPressed[LSTICK] && !(GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
	ButtonReleased[RSTICK] = ButtonPressed[RSTICK] && !(GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
	ButtonReleased[DPAD_UP] = ButtonPressed[DPAD_UP] && !(GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
	ButtonReleased[DPAD_DOWN] = ButtonPressed[DPAD_DOWN] && !(GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
	ButtonReleased[DPAD_LEFT] = ButtonPressed[DPAD_LEFT] && !(GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
	ButtonReleased[DPAD_RIGHT] = ButtonPressed[DPAD_RIGHT] && !(GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);

	ButtonPressed[BUTTON_A] = GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_A;
	ButtonPressed[BUTTON_B] = GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_B;
	ButtonPressed[BUTTON_X] = GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_X;
	ButtonPressed[BUTTON_Y] = GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_Y;
	ButtonPressed[BUTTON_RT] = GetCurrState().Gamepad.bRightTrigger;
	ButtonPressed[BUTTON_RB] = GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
	ButtonPressed[BUTTON_LT] = GetCurrState().Gamepad.bLeftTrigger;
	ButtonPressed[BUTTON_LB] = GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
	ButtonPressed[BUTTON_START] = GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_START;
	ButtonPressed[BUTTON_BACK] = GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK;
	ButtonPressed[LSTICK_UP] = GetCurrState().Gamepad.sThumbLY > JUMPSENSITIVITY; //XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	ButtonPressed[LSTICK_DOWN] = GetCurrState().Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	ButtonPressed[LSTICK_LEFT] = GetCurrState().Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	ButtonPressed[LSTICK_RIGHT] = GetCurrState().Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	ButtonPressed[LSTICK] = GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
	ButtonPressed[RSTICK] = GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
	ButtonPressed[DPAD_UP] = GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
	ButtonPressed[DPAD_DOWN] = GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
	ButtonPressed[DPAD_LEFT] = GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
	ButtonPressed[DPAD_RIGHT] = GetCurrState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
}

void XBoxInputSystem::Update()
{
	m_player->Update();
}

bool XBoxInputSystem::isKeyPressed(ControllerButtons key)
{
	return m_player->ButtonPressed[key];
}

bool XBoxInputSystem::isKeyTriggered(ControllerButtons key)
{
	return m_player->ButtonTriggered[key];
}

bool XBoxInputSystem::isKeyReleased(ControllerButtons key)
{
	return m_player->ButtonReleased[key];
}

bool XBoxInputSystem::isLStickCurr(int dir)
{
	//0 = up, 1 = down, 2 = left, 3 = right
	switch (dir)
	{
	case(1): return isKeyPressed(LSTICK_UP);
	case(2): return isKeyPressed(LSTICK_DOWN);
	case(3): return isKeyPressed(LSTICK_LEFT);
	case(4): return isKeyPressed(LSTICK_RIGHT);
	}
	return false;
}

bool XBoxInputSystem::isLStickTriggered(int dir)
{
	//0 = up, 1 = down, 2 = left, 3 = right
	switch (dir)
	{
	case(1): return isKeyTriggered(LSTICK_UP);
	case(2): return isKeyTriggered(LSTICK_DOWN);
	case(3): return isKeyTriggered(LSTICK_LEFT);
	case(4): return isKeyTriggered(LSTICK_RIGHT);
	}

	return false;
}

bool XBoxInputSystem::isLStickReleased(int dir)
{
	switch (dir)
	{
	case(1): return isKeyReleased(LSTICK_UP);
	case(2): return isKeyReleased(LSTICK_DOWN);
	case(3): return isKeyReleased(LSTICK_LEFT);
	case(4): return isKeyReleased(LSTICK_RIGHT);
	}

	return false;
}

bool XBoxInputSystem::isLTRTCurr(int dir)
{
	switch (dir)
	{
	case(1): return isKeyPressed(BUTTON_LT);
	case(2): return isKeyPressed(BUTTON_RT);
	}

	return false;
}

bool XBoxInputSystem::isLTRTTriggered(int dir)
{
	switch (dir)
	{
	case(1): return isKeyTriggered(BUTTON_LT);
	case(2): return isKeyTriggered(BUTTON_RT);
	}

	return false;
}


bool XBoxInputSystem::isLTRTReleased(int dir)
{
	switch (dir)
	{
	case(1): return isKeyReleased(BUTTON_LT);
	case(2): return isKeyReleased(BUTTON_RT);
	}
	return false;
}


void XBoxInputSystem::AddToScript(sol::state * luaState)
{
	luaState->new_usertype<XBoxInputSystem>("xbox",
		"KeyDown", &XBoxInputSystem::isKeyPressed,
		"KeyUp", &XBoxInputSystem::isKeyReleased,
		"KeyTrig", &XBoxInputSystem::isKeyTriggered,
		"LStickPressed", &XBoxInputSystem::isLStickCurr,
		"LStickTriggered", &XBoxInputSystem::isLStickTriggered,
		"LStickReleased", &XBoxInputSystem::isLStickReleased,
		"LTRTPressed", &XBoxInputSystem::isLTRTCurr,
		"LTRTTriggered", &XBoxInputSystem::isLTRTTriggered,
		"LTRTReleased", &XBoxInputSystem::isLTRTReleased
		);
}

