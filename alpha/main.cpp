﻿
#include<vector>

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"

#include "Object3d.h"
#include "Model.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "DebugCamera.h"
#include "Collision.h"

using namespace DirectX;
using XMFLOAT2 = DirectX::XMFLOAT2;
using XMFLOAT3 = DirectX::XMFLOAT3;
using XMFLOAT4 = DirectX::XMFLOAT4;
using XMVECTOR = DirectX::XMVECTOR;
using XMMATRIX = DirectX::XMMATRIX;

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

	Object3d::StaticInitialize(dxCommon->GetDev());
	DebugCamera* camera = nullptr;
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height, input);
	Object3d::SetCamera(camera);

	// カメラ注視点をセット
	camera->SetTarget({ 0, 1, 500 });
	camera->SetDistance(3.0f);

#pragma endregion DirectX初期化処理

#pragma region 描画初期化処理

	SpriteCommon* spriteCommon = new SpriteCommon();
	spriteCommon->initialize(dxCommon->GetDev(), dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);

	spriteCommon->LoadTexture(0, L"Resources/texture.png");
	spriteCommon->LoadTexture(1, L"Resources/house.png");
	spriteCommon->LoadTexture(2, L"Resources/player.png");

	std::vector<Sprite*> sprites;
	/*Sprite* sprite = Sprite::Create(spriteCommon, 0);
	sprites.push_back(sprite);
	sprite->SetPosition({ 500,300,0 });*/

	for (int i = 0; i < 20; i++)
	{
		int texNum = rand() % 2;

		Sprite* sprite = Sprite::Create(spriteCommon, 2, { 0,0 }, false, false);

		//sprite->SetPosition({ (float)(rand() % 1280),(float)(rand() % 720),0 });

		//sprite->SetRotation((float)(rand() % 360));

		//sprite->SetSize({ (float)(rand() % 400), (float)(rand() % 100) });

		sprite->TransferVertexBuffer();

		sprites.push_back(sprite);
		//sprite->SetPosition({ 500,300,0 });
	}

	Model* modelPost = Model::LoadFromOBJ("posuto");
	Model* modelChr = Model::LoadFromOBJ("chr_sword");
	Model* modelThunder = Model::LoadFromOBJ("solothunder");
	Model* modelCloud = Model::LoadFromOBJ("cloud");
	Model* modelEnemyMovA = Model::LoadFromOBJ("rat");
	Model* modelEnemyMovB = Model::LoadFromOBJ("greenbox");
	Model* modelEnemyMovC = Model::LoadFromOBJ("bluebox");
	Object3d* objEnemyMov[10] = {};
	XMFLOAT3 enemyMovPos[10];

	int enemyNam = 10;

	int enemyFlag[10] = {};

	const float PI = 3.1415926f;

	Object3d* objPost = Object3d::Create();
	Object3d* objChr = Object3d::Create();
	Object3d* player = Object3d::Create();
	Object3d* objThunder = Object3d::Create();
	Object3d* objCloud = Object3d::Create();

	player->SetModel(modelChr);
	objPost->SetModel(modelPost);
	objChr->SetModel(modelChr);
	objThunder->SetModel(modelThunder);
	objCloud->SetModel(modelCloud);

	objPost->SetPosition({ 0,0,50 });
	objChr->SetPosition({ 0,-25,-75 });

	for (int i = 0; i < enemyNam; i++)
	{
		objEnemyMov[i] = Object3d::Create();
		objEnemyMov[i]->SetModel(modelEnemyMovA);
		XMFLOAT3 vel{};
		const float rnd_acc = 0.0000f;
		float radius = (float)(rand() % 600);
		vel.x = cos((((float)(rand() % 360)) * PI) / 180) * radius;
		vel.y = 0.0f;
		vel.z = sin((((float)(rand() % 360)) * PI) / 180) * radius;
		enemyMovPos[i] = vel;
		objEnemyMov[i]->SetPosition(enemyMovPos[i]);
		objEnemyMov[i]->Update();
	}
	//player->SetPosition({ 0.0f,0.0f,0.0f });
	objThunder->SetPosition({ 0.0f,100.0f,0.0f });
	objCloud->SetPosition({ 0.0f,1.0f,480.0f });
	camera->SetEye({ 0, 0, -20 });

	/*objPost->Update();
	objChr->Update();*/

#pragma endregion 描画初期化処理

	int counter = 0; // アニメーションの経過時間カウンター

	// カメラ関係
	bool dirty = false;
	float angleX = 0;
	float angleY = 0;
	float scaleX = 1.0f / (float)WinApp::window_width;
	float scaleY = 1.0f / (float)WinApp::window_height;
	bool viewDirty = false;
	float distance = 20.0f;
	XMMATRIX matRot = DirectX::XMMatrixIdentity();

	// 最短距離関係
	float earliest[2];
	XMFLOAT3 Earliest;
	int earliestEnemyNum;

	// 雷関係
	XMFLOAT3 thunderPos = objThunder->GetPosition();
	int thunderFlag = 0;
	int thunderTimer = 0;

	// 雲関係
	XMFLOAT3 cloudPos = objCloud->GetPosition();
	XMFLOAT3 cloudRot = objCloud->GetRotation();

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

		// マウスの入力を取得
		Input::MouseMove mouseMove = input->GetMouseMove();
		float dy = mouseMove.lX * scaleY;
		angleY = -dy * XM_PI;

		// ボタンが押されていたらカメラを並行移動させる
		if (input->PushKey(DIK_D))
		{
			XMVECTOR move = { 3.0f, 0, 0, 0 };
			move = XMVector3Transform(move, matRot);
			camera->MoveVector(move);
		}
		if (input->PushKey(DIK_A))
		{
			XMVECTOR move = { -3.0f, 0, 0, 0 };
			move = XMVector3Transform(move, matRot);
			camera->MoveVector(move);
		}
		if (input->PushKey(DIK_W))
		{
			XMVECTOR move = { 0, 0, 3.0f, 0 };
			move = XMVector3Transform(move, matRot);
			camera->MoveVector(move);
		}
		if (input->PushKey(DIK_S))
		{
			XMVECTOR move = { 0, 0, -3.0f, 0 };
			move = XMVector3Transform(move, matRot);
			camera->MoveVector(move);
		}

		// 追加回転分の回転行列を生成
		XMMATRIX matRotNew = XMMatrixIdentity();
		matRotNew *= XMMatrixRotationY(-angleY);
		// 累積の回転行列を合成
		// ※回転行列を累積していくと、誤差でスケーリングがかかる危険がある為
		// クォータニオンを使用する方が望ましい
		matRot = matRotNew * matRot;

		// 注視点から視点へのベクトルと、上方向ベクトル
		XMVECTOR vTargetEye = { 0.0f, 0.0f, -distance, 1.0f };
		XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };

		// ベクトルを回転
		vTargetEye = XMVector3Transform(vTargetEye, matRot);
		vUp = XMVector3Transform(vUp, matRot);

		// 長さ
		float length = 0.0f;

		XMFLOAT3 target1 = camera->GetTarget();
		camera->SetEye({ target1.x + vTargetEye.m128_f32[0], target1.y + vTargetEye.m128_f32[1], target1.z + vTargetEye.m128_f32[2] });
		camera->SetUp({ vUp.m128_f32[0], vUp.m128_f32[1], vUp.m128_f32[2] });

		// 注視点からずらした位置に視点座標を決定
		XMFLOAT3 target2 = camera->GetTarget();
		XMFLOAT3 eye = camera->GetEye();

		XMFLOAT3 fTargetEye = { 0.0f, 0.0f, 0.0f };

		// 大きさ計算
		length = sqrtf(pow(target2.x - eye.x, 2) + pow(target2.y - eye.y, 2) + pow(target2.z - eye.z, 2));
		fTargetEye.x = eye.x - target2.x;
		fTargetEye.y = eye.y - target2.y;
		fTargetEye.z = eye.z - target2.z;

		fTargetEye.x /= length;
		fTargetEye.y /= length;
		fTargetEye.z /= length;

		fTargetEye.x *= 17;
		fTargetEye.y *= 17;
		fTargetEye.z *= 17;

		//vTargetEye2.m128_f32[0] = sqrt(pow(target.x - eye.x, 2));
		//vTargetEye2.m128_f32[1] = sqrt(pow(target.y - eye.y, 2));
		//vTargetEye2.m128_f32[2] = sqrt(pow(target.z - eye.z, 2));
		////                         ↓おかしい。ベクトルの大きさで割る
		//double vTargetEyeX = 1 / vTargetEye2.m128_f32[0];
		//double vTargetEyeY = 1 / vTargetEye2.m128_f32[1];
		//double vTargetEyeZ = 1 / vTargetEye2.m128_f32[2];
		//// 正規化　　　　　　　　　↓おかしい。target - eye のベクトルがただしい。
		//vTargetEye2.m128_f32[0] = vTargetEye2.m128_f32[0] * vTargetEyeX;
		//vTargetEye2.m128_f32[1] = vTargetEye2.m128_f32[1] * vTargetEyeY;
		//vTargetEye2.m128_f32[2] = vTargetEye2.m128_f32[2] * vTargetEyeZ;

		// 前提。この時点で大きさが1になった状態のtargetからeyeへ向かうベクトルが出ている。

		objCloud->SetScale({ 1.0f, 1.0f, 1.0f });
		cloudPos = { target2.x + fTargetEye.x, target2.y + fTargetEye.y - 1.5f, target2.z + fTargetEye.z };
		objCloud->SetPosition(cloudPos);

		cloudRot.y = atan2f(-fTargetEye.x, -fTargetEye.z);
		cloudRot.y *= 180 / PI;
		objCloud->SetRotation({ 0.0f, cloudRot.y, 0.0f });

		// 最短距離を求める
		for (int i = 0; i < enemyNam; i++)
		{
			Earliest.x = target1.x - enemyMovPos[i].x;
			Earliest.y = target1.y - enemyMovPos[i].y;
			Earliest.z = target1.z - enemyMovPos[i].z;

			if (enemyFlag[i] == 0)
			{
				if (i == 0)
				{
					earliest[0] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));
					earliestEnemyNum = i;
				}
				if (i > 0)
				{
					earliest[1] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

					if (earliest[0] > earliest[1])
					{
						earliestEnemyNum = i;
						earliest[0] = earliest[1];
					}
					if (earliest[0] < earliest[1])
					{
						earliestEnemyNum = earliestEnemyNum;
					}
				}
			}
			// earliest[0]が最短距離 earliestEnemyNumがenemyMovのナンバー
		}

		// 攻撃処理
		if (input->TriggerMouseLeft() && thunderFlag == 0)
		{
			// 攻撃判定
			bool isTerritory = Collision::territory(target1, enemyMovPos[earliestEnemyNum]);
			if (isTerritory)
			{
				thunderFlag = 1;
				thunderPos = enemyMovPos[earliestEnemyNum];
				thunderPos.y += 100.0;
				thunderTimer = 10;
				objThunder->SetPosition(thunderPos);
			}
		}
		if (thunderFlag == 1)
		{
			thunderPos.y -= 20.0f;
			if (thunderPos.y <= 40)
			{
				enemyFlag[earliestEnemyNum] = 1;
				thunderFlag = 0;
			}
		}

		// 雷表示時間
		if (thunderTimer != 0) { thunderTimer--; }

		for (int i = 0; i < enemyNam; i++)
		{
			objEnemyMov[i]->Update();
		}

		if (input->PushKey(DIK_ESCAPE)) {
			break;
		}

		//SetCursorPos(860, 440);

		camera->Update();
		objPost->Update();
		player->Update();
		objChr->Update();
		objThunder->Update();
		objCloud->Update();

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

		//objChr->Draw();

		for (int i = 0; i < enemyNam; i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Draw(); }
		}
		//player->Draw();
		if (0 < thunderTimer) { objThunder->Draw(); }

		objCloud->Draw();

		Object3d::PostDraw();

		spriteCommon->PreDraw();
		/*for (auto& sprite : sprites)
		{
			sprite->Draw();
		}*/

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
	delete modelThunder;
	delete modelCloud;
	delete modelEnemyMovA;
	delete modelEnemyMovB;
	delete modelEnemyMovC;
	delete objChr;
	delete objPost;
	delete objThunder;
	delete objCloud;
	for (int i = 0; i < enemyNam; i++)
	{
		delete objEnemyMov[i];
	}
	return 0;
}