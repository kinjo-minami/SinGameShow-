#include "Input.h"



#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


void Input::Initialize(WinApp* winApp)
{
	HRESULT result;
	this->winApp = winApp;
	result = DirectInput8Create(
		winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);

	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);

	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式

	result = devkeyboard->SetCooperativeLevel(
		winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);

	result = devMouse->SetDataFormat(&c_dfDIMouse2); // 標準形式

	result = devMouse->SetCooperativeLevel(
		winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	result = dinput->CreateDevice(GUID_Joystick, &devJoystick, NULL);

	result = devJoystick->SetDataFormat(&c_dfDIJoystick2); // 標準形式

	result = devJoystick->SetCooperativeLevel(
		winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
}

void Input::Update()
{
	HRESULT result;

	memcpy(keyPre, key, sizeof(key));

	result = devkeyboard->Acquire();

	result = devkeyboard->GetDeviceState(sizeof(key), key);

	result = devMouse->Acquire();

	mouseStatePre = mouseState;

	result = devMouse->GetDeviceState(sizeof(mouseState), &mouseState);

	result = devJoystick->Acquire();

	joyStatePre = joyState;

	result = devJoystick->GetDeviceState(sizeof(joyState), &joyState);
}

bool Input::PushKey(BYTE keyNumber)
{
	if (key[keyNumber]) {
		return true;
	}
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	if (key[keyNumber] == 0x80 && keyPre[keyNumber] == 0x00) {
		return true;
	}
	return false;
}

bool Input::PushMouseLeft()
{
	if (mouseState.rgbButtons[0]) {
		return true;
	}
	return false;
}

bool Input::PushMouseRight()
{
	if (mouseState.rgbButtons[1]) {
		return true;
	}
	return false;
}

bool Input::PushMouseMiddle()
{
	if (mouseState.rgbButtons[2]) {
		return true;
	}
	return false;
}

bool Input::TriggerMouseLeft()
{
	if (!mouseStatePre.rgbButtons[0] && mouseState.rgbButtons[0]) {
		return true;
	}
	return false;
}

bool Input::TriggerMouseRight()
{
	if (!mouseStatePre.rgbButtons[1] && mouseState.rgbButtons[1]) {
		return true;
	}
	return false;
}

bool Input::TriggerMouseMiddle()
{
	if (!mouseStatePre.rgbButtons[2] && mouseState.rgbButtons[2]) {
		return true;
	}
	return false;
}

Input::MouseMove Input::GetMouseMove()
{
	MouseMove tmp;
	tmp.lX = mouseState.lX;
	tmp.lY = mouseState.lY;
	tmp.lZ = mouseState.lZ;
	return tmp;
}
