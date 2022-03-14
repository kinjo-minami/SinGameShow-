#include "Input.h"



#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


Input* Input::GetInstance() {
    static Input instance;

    return &instance;
}

void Input::Initialize(WinApp* winApp) {
    HRESULT result;
    this->winApp = winApp;
    result = DirectInput8Create(
        winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);

    result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);

    result = devkeyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式

    result = devkeyboard->SetCooperativeLevel(
        winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
}

void Input::Update() {
    HRESULT result;

    memcpy(keyPre, key, sizeof(key));

    result = devkeyboard->Acquire();

    result = devkeyboard->GetDeviceState(sizeof(key), key);
}

bool Input::PushKey(BYTE keyNumber) {
    if (key[keyNumber]) {
        return true;
    }
    return false;
}

bool Input::TriggerKey(BYTE keyNumber) {
    if (key[keyNumber] == 0x80 && keyPre[keyNumber] == 0x00) {
        return true;
    }
    return false;
}
