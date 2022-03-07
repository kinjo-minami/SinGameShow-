#include "Framework.h"

void Framework::Run()
{
	Initialize();

	while (true)  // �Q�[�����[�v
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

#pragma region WindowsAPI������

	winApp = new WinApp();
	winApp->Initialize();

	MSG msg{};  // ���b�Z�[�W

#pragma endregion WindowsAPI������

#pragma region DirectX����������


	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	input = new Input();
	input->Initialize(winApp);
	
	spriteCommon = new SpriteCommon();

	Object3d::StaticInitialize(dxCommon->GetDev(), winApp->window_width, winApp->window_height);

#pragma endregion DirectX����������

}

void Framework::Finalize()
{
	// XAudio2���
// xAudio2.Reset();
 // �����f�[�^���
// SoundUnload(&soundData1);

	delete input;
	delete spriteCommon;
#pragma region WindowsAPI��n��
	winApp->Finalize();
#pragma endregion WindowsAPI��n��
	delete winApp;
}

void Framework::Update()
{
#pragma region �E�B���h�E���b�Z�[�W����
	if (winApp->ProcessMessage()) {
		gameloopEndReqest_ = true;
		return;
	}
#pragma endregion �E�B���h�E���b�Z�[�W����
	input->Update();

}

void Framework::Draw()
{
}
