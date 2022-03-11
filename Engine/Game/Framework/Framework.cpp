#include "Framework.h"

void Framework::Run()
{
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

void Framework::Initialize()
{

#pragma region WindowsAPI初期化

	winApp = new WinApp();
	winApp->Initialize();

	MSG msg{};  // メッセージ

#pragma endregion WindowsAPI初期化

#pragma region DirectX初期化処理


	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	input = new Input();
	input->Initialize(winApp);
	
	spriteCommon = new SpriteCommon();

	Object3d::StaticInitialize(dxCommon->GetDev(), winApp->window_width, winApp->window_height);

#pragma endregion DirectX初期化処理

}

void Framework::Finalize()
{
	// XAudio2解放
// xAudio2.Reset();
 // 音声データ解放
// SoundUnload(&soundData1);

	delete input;
	delete spriteCommon;
#pragma region WindowsAPI後始末
	winApp->Finalize();
#pragma endregion WindowsAPI後始末
	delete winApp;
}

void Framework::Update()
{
#pragma region ウィンドウメッセージ処理
	if (winApp->ProcessMessage()) {
		gameloopEndReqest_ = true;
		return;
	}
#pragma endregion ウィンドウメッセージ処理
	input->Update();

}

void Framework::Draw()
{
}
