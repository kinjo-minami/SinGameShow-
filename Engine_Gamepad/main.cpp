﻿
#include<vector>

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"

#include "Object3d.h"
#include "Model.h"
#include"SpriteCommon.h"
#include"Sprite.h"

using XMFLOAT2 = DirectX::XMFLOAT2;
using XMFLOAT3 = DirectX::XMFLOAT3;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

#pragma region WindowsAPI初期化
	WinApp* winApp = nullptr;

	winApp = new WinApp();
	winApp->Initialize();

	MSG msg{};  // メッセージ
#pragma endregion WindowsAPI初期化

#pragma region DirectX初期化処理

	DirectXCommon* dxCommon = nullptr;

	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	Input* input = nullptr;
	input = new Input();
	input->Initialize(winApp);

	Object3d::StaticInitialize(dxCommon->GetDev(), winApp->window_width, winApp->window_height);

#pragma endregion DirectX初期化処理

#pragma region 描画初期化処理

	SpriteCommon* spriteCommon = new SpriteCommon();
	spriteCommon->initialize(dxCommon->GetDev(), dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);

	spriteCommon->LoadTexture(0, L"Resources/texture.png");
	spriteCommon->LoadTexture(1, L"Resources/house.png");

	std::vector<Sprite*> sprites;
	/*Sprite* sprite = Sprite::Create(spriteCommon, 0);
	sprites.push_back(sprite);
	sprite->SetPosition({ 500,300,0 });*/

	for (int i = 0; i < 20; i++)
	{
		int texNum = rand() % 2;

		Sprite* sprite = Sprite::Create(spriteCommon, texNum, { 0,0 }, false, false);

		sprite->SetPosition({ (float)(rand() % 1280),(float)(rand() % 720),0 });

		//sprite->SetRotation((float)(rand() % 360));

		sprite->SetSize({ (float)(rand() % 400), (float)(rand() % 100) });

		sprite->TransferVertexBuffer();

		sprites.push_back(sprite);
		//sprite->SetPosition({ 500,300,0 });

	}


	Model* modelPost = Model::LoadFromOBJ("posuto");
	Model* modelChr = Model::LoadFromOBJ("chr_sword");

	Object3d* objPost = Object3d::Create();
	Object3d* objChr = Object3d::Create();


	objPost->SetModel(modelPost);
	objChr->SetModel(modelPost);

	objPost->SetPosition({ -10,0,-5 });
	objChr->SetPosition({ +10,0,+5 });

	objPost->Update();
	objChr->Update();

#pragma endregion 描画初期化処理

	int counter = 0; // アニメーションの経過時間カウンター

	while (true)  // ゲームループ
	{
#pragma region ウィンドウメッセージ処理
		if (winApp->ProcessMessage()) {
			break;
		}
#pragma endregion ウィンドウメッセージ処理

#pragma region DirectX毎フレーム処理
		// DirectX毎フレーム処理　ここから

		input->Update();

		const int cycle = 540; // 繰り返しの周期
		counter++;
		counter %= cycle; // 周期を超えたら0に戻る
		float scale = (float)counter / cycle; // [0,1]の数値

		scale *= 360.0f;
		objPost->SetModel(modelPost);
		objChr->SetModel(modelPost);
		if (input->TriggerKey(DIK_0)) // 数字の0キーが押されていたら
		{
			OutputDebugStringA("Hit 0\n");  // 出力ウィンドウに「Hit 0」と表示
		}

		float clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // 青っぽい色

		if (input->PushKey(DIK_SPACE))     // スペースキーが押されていたら
		{
			// 画面クリアカラーの数値を書き換える
			clearColor[1] = 1.0f;
			objPost->SetModel(modelChr);
			objChr->SetModel(modelChr);
		}

		// 座標操作
		if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
		{

		}


		if (input->PushKey(DIK_D) || input->PushKey(DIK_A))
		{

		}

		XMFLOAT3 PostPosition = objPost->GetPosition();
		Input::MouseMove mouseMove = input->GetMouseMove();
		Input::StickMove stickMove = input->GetStickMove();
		if (input->PushButtonA()) { PostPosition.x -= 1.0f; }

		// マウスの左ボタンが押されていたら
		if (input->PushMouseLeft()) { PostPosition.x -= 1.0f; }
		// マウスの右ボタンが押されていたら
		if (input->PushMouseRight()) { PostPosition.x += 1.0f; }
		// ホイールを前に動かしたら
		if (mouseMove.lZ > 0) { PostPosition.z += 2.0f; }
		// ホイールを後ろに動かしたら
		if (mouseMove.lZ < 0) { PostPosition.z -= 2.0f; }

		// ゲームパッドのスティックテスト(最小0、最大65535、真ん中32767)
		if (stickMove.lX < 27767) { PostPosition.x -= 2.0f; }
		if (stickMove.lX > 37767) { PostPosition.x += 2.0f; }
		if (stickMove.lY < 27767) { PostPosition.y += 2.0f; }
		if (stickMove.lY > 37767) { PostPosition.y -= 2.0f; }

		// 右トリガー押し込み
		//if (stickMove.lZ == 128) { PostPosition.y -= 2.0f; }
		// 右トリガー押してない
		//if (stickMove.lZ == 32767) { PostPosition.y += 2.0f; }

		// 左トリガー押し込み
		//if (stickMove.lRz > 0) { PostPosition.y -= 2.0f; }
		// 左トリガー押してない
		//if (stickMove.lZ == -127) { PostPosition.y += 2.0f; }


		objPost->SetPosition(PostPosition);

		objPost->Update();
		objChr->Update();
		for (auto& sprite : sprites)
		{
			sprite->Update();
		}

		// DirectX毎フレーム処理　ここまで
#pragma endregion DirectX毎フレーム処理

#pragma region グラフィックスコマンド

		dxCommon->PreDraw();

		Object3d::PreDraw(dxCommon->GetCmdList());
		objPost->Draw();
		objChr->Draw();
		Object3d::PostDraw();

		spriteCommon->PreDraw();
		for (auto& sprite : sprites)
		{
			//sprite->Draw();
		}

		// ４．描画コマンドここまで
		dxCommon->PostDraw();


	}
	// XAudio2解放
   // xAudio2.Reset();
	// 音声データ解放
   // SoundUnload(&soundData1);


#pragma region WindowsAPI後始末
	winApp->Finalize();
#pragma endregion WindowsAPI後始末
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
	return 0;
}