#pragma once
#include <windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION     0x0800   // DirectInput�̃o�[�W�����w��
#include <dinput.h>

#include "WinApp.h"

class Input
{
public:
	struct MouseMove {
		LONG    lX;
		LONG    lY;
		LONG    lZ;
	};
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	//������
	void Initialize(WinApp* winApp);
	//�X�V
	void Update();
	//�L�[�̉������`�F�b�N
	bool PushKey(BYTE keyNumber);
	//�L�[�̃g���K�[���`�F�b�N
	bool TriggerKey(BYTE keyNumber);

	// �}�E�X�̍��{�^���������`�F�b�N
	bool PushMouseLeft();
	// �}�E�X�̉E�{�^���������`�F�b�N
	bool PushMouseRight();
	// �}�E�X�̒��{�^���������`�F�b�N
	bool PushMouseMiddle();

	// �}�E�X�̍��{�^���g���K�[���`�F�b�N
	bool TriggerMouseLeft();
	// �}�E�X�̉E�{�^���g���K�[���`�F�b�N
	bool TriggerMouseRight();
	// �}�E�X�̒��{�^���g���K�[���`�F�b�N
	bool TriggerMouseMiddle();

	// �}�E�X�ړ��ʂ��擾
	MouseMove GetMouseMove();
private:
	//windowsAPI
	WinApp* winApp = nullptr;

	ComPtr<IDirectInputDevice8> devkeyboard = nullptr;
	ComPtr<IDirectInput8> dinput = nullptr;

	BYTE key[256] = {};
	BYTE keyPre[256] = {};

	ComPtr<IDirectInputDevice8> devMouse;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 mouseStatePre = {};

	ComPtr<IDirectInputDevice8> devJoystick;
	DIJOYSTATE2 joyState = {};
	DIJOYSTATE2 joyStatePre = {};
};
