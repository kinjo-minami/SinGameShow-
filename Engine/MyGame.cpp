#include "MyGame.h"

void MyGame::Initialize()
{

#pragma region WindowsAPI������

	winApp = new WinApp();
	winApp->Initialize();

#pragma endregion WindowsAPI������

#pragma region DirectX����������


	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	input = new Input();
	input->Initialize(winApp);

	Object3d::StaticInitialize(dxCommon->GetDev(), winApp->window_width, winApp->window_height);

#pragma endregion DirectX����������

#pragma region �`�揉��������

	spriteCommon->initialize(dxCommon->GetDev(), dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);

	spriteCommon->LoadTexture(0, L"Resources/texture.png");
	spriteCommon->LoadTexture(1, L"Resources/house.png");

	/*Sprite* sprite = Sprite::Create(spriteCommon, 0);
	sprites.push_back(sprite);
	sprite->SetPosition({ 500,300,0 });*/

	for (int i = 0; i < 20; i++)
	{
		int texNum = rand() % 2;

		sprite = Sprite::Create(spriteCommon, texNum, { 0,0 }, false, false);

		sprite->SetPosition({ (float)(rand() % 1280),(float)(rand() % 720),0 });

		//sprite->SetRotation((float)(rand() % 360));

		sprite->SetSize({ (float)(rand() % 400), (float)(rand() % 100) });

		sprite->TransferVertexBuffer();

		sprites.push_back(sprite);
		//sprite->SetPosition({ 500,300,0 });

	}


	modelPost = Model::LoadFromOBJ("posuto");
	modelChr = Model::LoadFromOBJ("chr_sword");

	objPost = Object3d::Create();
	objChr = Object3d::Create();


	objPost->SetModel(modelPost);
	objChr->SetModel(modelPost);

	objPost->SetPosition({ -10,0,-5 });
	objChr->SetPosition({ +10,0,+5 });

	objPost->Update();
	objChr->Update();

#pragma endregion �`�揉��������
}

void MyGame::Finalize()
{
	// XAudio2���
// xAudio2.Reset();
 // �����f�[�^���
// SoundUnload(&soundData1);


#pragma region WindowsAPI��n��
	winApp->Finalize();
#pragma endregion WindowsAPI��n��
	delete input;
	delete winApp;
	delete spriteCommon;
	for (auto& sprite : sprites)
	{
		delete sprite;
	}
	//delete sprite;
	delete modelPost;
	delete modelChr;
	delete objChr;
	delete objPost;
}

void MyGame::Update()
{
#pragma region �E�B���h�E���b�Z�[�W����
	if (winApp->ProcessMessage()) {
		gameloopEndReqest_ = true;
		return;
	}
#pragma endregion �E�B���h�E���b�Z�[�W����
#pragma region DirectX���t���[������
	// DirectX���t���[�������@��������

	input->Update();

	const int cycle = 540; // �J��Ԃ��̎���
	counter++;
	counter %= cycle; // �����𒴂�����0�ɖ߂�
	float scale = (float)counter / cycle; // [0,1]�̐��l

	scale *= 360.0f;
	objPost->SetModel(modelPost);
	objChr->SetModel(modelPost);
	if (input->TriggerKey(DIK_0)) // ������0�L�[��������Ă�����
	{
		OutputDebugStringA("Hit 0\n");  // �o�̓E�B���h�E�ɁuHit 0�v�ƕ\��
	}

	float clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // ���ۂ��F

	if (input->PushKey(DIK_SPACE))     // �X�y�[�X�L�[��������Ă�����
	{
		// ��ʃN���A�J���[�̐��l������������
		clearColor[1] = 1.0f;
		objPost->SetModel(modelChr);
		objChr->SetModel(modelChr);
	}

	// ���W����
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{

	}


	if (input->PushKey(DIK_D) || input->PushKey(DIK_A))
	{

	}

	objPost->Update();
	objChr->Update();
	for (auto& sprite : sprites)
	{
		sprite->Update();
	}

	// DirectX���t���[�������@�����܂�
#pragma endregion DirectX���t���[������
}

void MyGame::Draw()
{
#pragma region �O���t�B�b�N�X�R�}���h

	dxCommon->PreDraw();

	Object3d::PreDraw(dxCommon->GetCmdList());
	objPost->Draw();
	objChr->Draw();
	Object3d::PostDraw();

	spriteCommon->PreDraw();
	for (auto& sprite : sprites)
	{
		sprite->Draw();
	}

	// �S�D�`��R�}���h�����܂�
	dxCommon->PostDraw();

}
