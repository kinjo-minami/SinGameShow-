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
	struct JoyMove {
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

	// �Q�[���p�b�h��A�{�^���������`�F�b�N
	bool PushButtonA();
	// �Q�[���p�b�h��B�{�^���������`�F�b�N
	bool PushButtonB();
	// �Q�[���p�b�h��X�{�^���������`�F�b�N
	bool PushButtonX();
	// �Q�[���p�b�h��Y�{�^���������`�F�b�N
	bool PushButtonY();
	// �Q�[���p�b�h��LB�{�^���������`�F�b�N
	bool PushButtonLB();
	// �Q�[���p�b�h��RB�{�^���������`�F�b�N
	bool PushButtonRB();
	// �Q�[���p�b�h��Back�{�^���������`�F�b�N
	bool PushButtonBack();
	// �Q�[���p�b�h��Start�{�^���������`�F�b�N
	bool PushButtonStart();
	// �Q�[���p�b�h�̍��X�e�B�b�N�������`�F�b�N
	bool PushStickLeft();
	// �Q�[���p�b�h�̉E�X�e�B�b�N�������`�F�b�N
	bool PushStickRight();
	// �Q�[���p�b�h�̍��{�^���������`�F�b�N
	bool PushButtonLeft();

	// �Q�[���p�b�h�ړ��ʂ��擾
	//JoyMove GetJoyMove();
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

