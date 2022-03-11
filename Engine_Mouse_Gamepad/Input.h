#pragma once
#include <windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION     0x0800   // DirectInputのバージョン指定
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
	//初期化
	void Initialize(WinApp* winApp);
	//更新
	void Update();
	//キーの押下をチェック
	bool PushKey(BYTE keyNumber);
	//キーのトリガーをチェック
	bool TriggerKey(BYTE keyNumber);

	// マウスの左ボタン押下をチェック
	bool PushMouseLeft();
	// マウスの右ボタン押下をチェック
	bool PushMouseRight();
	// マウスの中ボタン押下をチェック
	bool PushMouseMiddle();

	// マウスの左ボタントリガーをチェック
	bool TriggerMouseLeft();
	// マウスの右ボタントリガーをチェック
	bool TriggerMouseRight();
	// マウスの中ボタントリガーをチェック
	bool TriggerMouseMiddle();

	// マウス移動量を取得
	MouseMove GetMouseMove();

	// ゲームパッドのAボタン押下をチェック
	bool PushButtonA();
	// ゲームパッドのBボタン押下をチェック
	bool PushButtonB();
	// ゲームパッドのXボタン押下をチェック
	bool PushButtonX();
	// ゲームパッドのYボタン押下をチェック
	bool PushButtonY();
	// ゲームパッドのLBボタン押下をチェック
	bool PushButtonLB();
	// ゲームパッドのRBボタン押下をチェック
	bool PushButtonRB();
	// ゲームパッドのBackボタン押下をチェック
	bool PushButtonBack();
	// ゲームパッドのStartボタン押下をチェック
	bool PushButtonStart();
	// ゲームパッドの左スティック押下をチェック
	bool PushStickLeft();
	// ゲームパッドの右スティック押下をチェック
	bool PushStickRight();
	// ゲームパッドの左ボタン押下をチェック
	bool PushButtonLeft();

	// ゲームパッド移動量を取得
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

