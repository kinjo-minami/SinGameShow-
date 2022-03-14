#include "Framework.h"

void Framework::Run() {
	Initialize();

	while (true)  // ゲームループ
	{
		Update();
		if (GetGameloopEndReqest() == true) {
			break;
		}
		Draw();
	}

	Finalize();
}

void Framework::Initialize() {

#pragma region WindowsAPI初期化

	winApp_ = new WinApp();
	winApp_->Initialize();

	MSG msg{};  // メッセージ

#pragma endregion WindowsAPI初期化

#pragma region DirectX初期化処理


	dxCommon_ = new DirectXCommon();
	dxCommon_->Initialize(winApp_);

	input_ = new Input();
	input_->Initialize(winApp_);
	
	spriteCommon_ = new SpriteCommon();
	spriteCommon_->initialize(dxCommon_->GetDev(), dxCommon_->GetCmdList(), winApp_->window_width, winApp_->window_height);


	Object3d::StaticInitialize(dxCommon_->GetDev(), winApp_->window_width, winApp_->window_height);

#pragma endregion DirectX初期化処理

}

void Framework::Finalize() {
	// XAudio2解放
// xAudio2.Reset();
 // 音声データ解放
// SoundUnload(&soundData1);

	delete input_;
	delete spriteCommon_;
#pragma region WindowsAPI後始末
	winApp_->Finalize();
#pragma endregion WindowsAPI後始末
	delete winApp_;
}

void Framework::Update() {
#pragma region ウィンドウメッセージ処理
	if (winApp_->ProcessMessage()) {
		gameloopEndReqest_ = true;
		return;
	}
#pragma endregion ウィンドウメッセージ処理
	input_->Update();

}

void Framework::Draw() {

}
