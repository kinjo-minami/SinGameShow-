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


	dxCommon_ = DirectXCommon::GetInstance();
	dxCommon_->Initialize(winApp_);

	input_ = Input::GetInstance();
	input_->Initialize(winApp_);
	
	spriteCommon_ = SpriteCommon::GetInstance();
	spriteCommon_->initialize(dxCommon_->GetDev(), dxCommon_->GetCmdList(), winApp_->window_width, winApp_->window_height);

	Object3d::StaticInitialize(dxCommon_->GetDev());
	
	sceneManager_ = new SceneManager();
#pragma endregion DirectX初期化処理

}

void Framework::Finalize() {
	// XAudio2解放
// xAudio2.Reset();
 // 音声データ解放
// SoundUnload(&soundData1);

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
	sceneManager_->Update();
}

void Framework::Draw() {
	dxCommon_->PreDraw();
	Object3d::PreDraw(dxCommon_->GetCmdList());
	sceneManager_->Draw();
	Object3d::PostDraw();
	dxCommon_->PostDraw();
}
