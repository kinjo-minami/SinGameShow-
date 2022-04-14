
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
#include"Audio.h"
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
	camera->SetTarget({ 0, 1, -100 });
	camera->SetDistance(3.0f);

#pragma endregion DirectX初期化処理

#pragma region 描画初期化処理

	SpriteCommon* spriteCommon = new SpriteCommon();
	spriteCommon->initialize(dxCommon->GetDev(), dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);

	spriteCommon->LoadTexture(0, L"Resources/texture.png");
	spriteCommon->LoadTexture(1, L"Resources/house.png");
	spriteCommon->LoadTexture(2, L"Resources/player.png");
	spriteCommon->LoadTexture(3, L"Resources/hp_bar.png");
	spriteCommon->LoadTexture(4, L"Resources/mouse.png");
	spriteCommon->LoadTexture(5, L"Resources/title.png");
	spriteCommon->LoadTexture(6, L"Resources/gameClear.png");
	spriteCommon->LoadTexture(7, L"Resources/gameover.png");

	std::vector<Sprite*> sprites;
	/*Sprite* sprite = Sprite::Create(spriteCommon, 0);
	sprites.push_back(sprite);
	sprite->SetPosition({ 500,300,0 });*/
	Sprite* spritePlayer = Sprite::Create(spriteCommon, 2, { 0,0 }, false, false);
	Sprite* spriteTitle = Sprite::Create(spriteCommon, 5, { 0,0 }, false, false);
	spritePlayer->SetSize({ 1280 ,720 });
	spritePlayer->TransferVertexBuffer();
	spriteTitle->SetSize({ 1280 ,720 });

	//spriteTitle->Update();
	spriteTitle->TransferVertexBuffer();
	Sprite* spriteMouse = Sprite::Create(spriteCommon, 4, { 0,0 }, false, false);
	Sprite* spriteClear = Sprite::Create(spriteCommon, 6, { 0,0 }, false, false);
	Sprite* spriteOver = Sprite::Create(spriteCommon, 7, { 0,0 }, false, false);

	//for (int i = 0; i < 20; i++)
	//{
	//	int texNum = rand() % 2;


	//	//sprite->SetPosition({ (float)(rand() % 1280),(float)(rand() % 720),0 });

	//	//sprite->SetRotation((float)(rand() % 360));

	//	//sprite->SetSize({ (float)(rand() % 400), (float)(rand() % 100) });

	//	sprite->TransferVertexBuffer();

	//	sprites.push_back(sprite);
	//	//sprite->SetPosition({ 500,300,0 });
	//}

	Model* inCoa = Model::LoadFromOBJ("core_in");
	Model* outCoa1 = Model::LoadFromOBJ("core_out1");
	Model* outCoa2 = Model::LoadFromOBJ("core_out2");

	Model* modelGround = Model::LoadFromOBJ("ground");

	Model* modelChr = Model::LoadFromOBJ("chr_sword");
	Model* modelThunder = Model::LoadFromOBJ("solothunder");
	Model* modelEnemyMovA = nullptr;
	Model* modelEnemyMovB = nullptr;
	Model* modelEnemyMovC = nullptr;
	Model* modelBack = Model::LoadFromOBJ("back");
	Object3d* objEnemyMov[10] = {};
	XMFLOAT3 enemyMovPos[10] = {};
	for (int i = 0; i < 10; i++)
	{
		enemyMovPos[i] = { 0,4,50 };
	}
	int enemyNam = 10;

	int ECount = 10;

	int enemyFlag[10] = {};

	const float PI = 3.1415926f;

	modelEnemyMovA = Model::LoadFromOBJ("rat");
	modelEnemyMovB = Model::LoadFromOBJ("greenbox");
	modelEnemyMovC = Model::LoadFromOBJ("bluebox");

	Object3d* OBJInCoa = Object3d::Create();
	Object3d* OBJOutCoaA = Object3d::Create();
	Object3d* OBJOutCoaB = Object3d::Create();
	Object3d* OBJBack = Object3d::Create();


	Object3d* objChr = Object3d::Create();
	Object3d* player = Object3d::Create();
	Object3d* objThunder = Object3d::Create();
	Object3d* objGround = Object3d::Create();


	player->SetModel(modelChr);
	OBJInCoa->SetModel(inCoa);
	OBJOutCoaA->SetModel(outCoa1);
	OBJOutCoaB->SetModel(outCoa2);
	OBJBack->SetModel(modelBack);
	objGround->SetModel(modelGround);


	objChr->SetModel(modelChr);
	objThunder->SetModel(modelThunder);

	OBJInCoa->SetPosition({ 0,4,50 });
	OBJOutCoaA->SetPosition({ 0,4,50 });
	OBJOutCoaB->SetPosition({ 0,4,50 });

	OBJBack->SetPosition({ 0,0,50 });
	objGround->SetPosition({ 0,-10,50 });
	objChr->SetPosition({ 0,-25,-75 });
	float radius = 500.0f;
	float angle[10] = {};

	for (int i = 0; i < enemyNam; i++)
	{
		objEnemyMov[i] = Object3d::Create();
		objEnemyMov[i]->SetModel(modelEnemyMovA);
		XMFLOAT3 vel{};
		float radY;
		//int ran = rand() % 360 + 1;
		int ran = 30 * i;
		angle[i] = (float)ran;
		//angle[i] = 60.0f;
		const float rnd_acc = 0.0000f;
		//float radius = (float)(rand() % 600);
		vel.x = sin((angle[i] * PI) / 180) * radius;
		vel.y = 0.0f;
		vel.z = cos((angle[i] * PI) / 180) * radius;
		enemyMovPos[i].x += vel.x;
		enemyMovPos[i].y += vel.y;
		enemyMovPos[i].z += vel.z;

		objEnemyMov[i]->SetRotation({ 0.0f,angle[i],0.0f });
		objEnemyMov[i]->SetPosition(enemyMovPos[i]);
		objEnemyMov[i]->Update();
	}
	player->SetPosition({ 0.0f,0.0f,0.0f });
	objThunder->SetPosition({ 0.0f,100.0f,0.0f });
	camera->SetEye({ 0, 0, 0 });

	/*objPost->Update();
	objChr->Update();*/
	OBJBack->SetScale({ 5.0f,5.0f,5.0f });
#pragma endregion 描画初期化処理

	int counter = 0; // アニメーションの経過時間カウンター

	int scene = 0;

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
	float earliest[10];
	XMFLOAT3 Earliest;
	int earliestEnemyNum;

	// 雷関係
	XMFLOAT3 thunderPos = objThunder->GetPosition();
	int thunderFlag = 0;
	int thunderTimer = 0;

	//コア係
	XMFLOAT3 CoaRotA = {};
	XMFLOAT3 CoaRotB = {};
	XMFLOAT3 CoaPos = { 0,4,50 };
	int coaHit = 10;

	//雨
	XMFLOAT3 rainPos[100] = {};
	float fallRainSpeed[100] = {};

	int rainFlag[100] = {};

	//雪
	XMFLOAT3 snowPos[100] = {};
	float snowXSpeed[100] = {};

	float fallSnowSpeed[100] = {};
	int snowFlag[100] = {};

	//音
	Audio* thunder = new Audio;
	thunder->Initialize();
	thunder->SoundLoadWave("Resources/BGM/thunder.wav");

	while (true)  // ゲームループ
	{
#pragma region ウィンドウメッセージ処理
		if (winApp->ProcessMessage()) {
			break;
		}
#pragma endregion ウィンドウメッセージ処理

#pragma region DirectX毎フレーム処理
		// DirectX毎フレーム処理　ここから
		SetCursorPos(860, 440);

		if (scene == 0)
		{
			if (input->TriggerMouseLeft())
			{
				scene = 1;

			}
			if (input->PushKey(DIK_ESCAPE))
			{
				break;
			}
		}

		if (scene == 1)
		{
			CoaRotA.y += 0.3f;
			CoaRotB.y -= 0.3f;

			OBJInCoa->SetRotation(CoaRotA);
			OBJOutCoaA->SetRotation(CoaRotB);
			OBJOutCoaB->SetRotation(CoaRotA);

			const int cycle = 540; // 繰り返しの周期
			counter++;
			counter %= cycle; // 周期を超えたら0に戻る
			float scale = (float)counter / cycle; // [0,1]の数値

			scale *= 360.0f;
			OBJInCoa->SetModel(inCoa);
			objChr->SetModel(modelChr);
			objThunder->SetModel(modelThunder);

			XMFLOAT3 playerPos = camera->GetTarget();
			//camera->SetTarget(CameraPos);

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

			if (input->PushKey(DIK_ESCAPE))
			{
				break;
			}


			dirty = true;

			if (dirty || viewDirty)
			{
				// 追加回転分の回転行列を生成
				XMMATRIX matRotNew = XMMatrixIdentity();
				matRotNew *= XMMatrixRotationX(-angleX);
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

				// 注視点からずらした位置に視点座標を決定
				const XMFLOAT3& target = camera->GetTarget();
				camera->SetEye({ target.x + vTargetEye.m128_f32[0], target.y + vTargetEye.m128_f32[1], target.z + vTargetEye.m128_f32[2] });
				camera->SetUp({ vUp.m128_f32[0], vUp.m128_f32[1], vUp.m128_f32[2] });
			}
			//エネミー移動
			/*for (int i = 0; i < enemyNam; i++)
			{
				XMFLOAT3 vel = {};
				vel.x = sin((angle[i] * PI) / 180) * 0.5f;
				vel.y = 0.0f;
				vel.z = cos((angle[i] * PI) / 180) * 0.5f;
				enemyMovPos[i].x -= vel.x;
				enemyMovPos[i].y -= vel.y;
				enemyMovPos[i].z -= vel.z;
				objEnemyMov[i]->SetPosition(enemyMovPos[i]);
				objEnemyMov[i]->Update();


			}*/

			//コアと敵のあたり判定
			for (int i = 0; i < enemyNam; i++)
			{
				bool CHit = Collision::CoaHit(CoaPos, enemyMovPos[i], 25);
				if (CHit && enemyFlag[i] == 0)
				{
					coaHit -= 1;
					enemyFlag[i] = 1;
					ECount--;
				}
			}

			if (coaHit <= 0)
			{
				scene = 2;
			}


			if (coaHit > 0 && ECount <= 0)
			{
				scene = 3;
			}


			// 最短距離を求める
			//for (int i = 0; i < enemyNam; i++)
			//{
			//

			//	if (enemyFlag[i] == 0)
			//	{
			//		
			//		/*if (i == 0)
			//		{
			//			earliest[0] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));
			//			earliestEnemyNum = i;
			//		}
			//		
			//		else if (i > 0)
			//		{
			//			earliest[1] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

			//			if (earliest[0] > earliest[1])
			//			{
			//				earliestEnemyNum = i;
			//				earliest[0] = earliest[1];
			//			}
			//			if (earliest[0] < earliest[1])
			//			{
			//				earliestEnemyNum = earliestEnemyNum;
			//			}
			//		}
			//		else
			//		{
			//			earliestEnemyNum = earliestEnemyNum;
			//		}*/
			//	}

			//
			//	// earliest[0]が最短距離 earliestEnemyNumがenemyMovのナンバー
			//}
			for (int i = 0; i < enemyNam; i++)
			{
				if (enemyFlag[i] == 0)
				{
					Earliest.x = playerPos.x - enemyMovPos[i].x;
					Earliest.y = playerPos.y - enemyMovPos[i].y;
					Earliest.z = playerPos.z - enemyMovPos[i].z;
					earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));
					if (i == 0)
					{
						earliestEnemyNum = 0;
					}
					else
					{
						if (earliest[earliestEnemyNum] > earliest[i])
						{
							earliestEnemyNum = i;
						}
						else if(earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] ==0)
						{
							earliestEnemyNum = earliestEnemyNum;
						}
						else
						{
							earliestEnemyNum = i;
						}
					}

				}


			}

			// 攻撃処理
			if (input->TriggerMouseLeft() && thunderFlag == 0)
			{
				// 攻撃判定
				bool isTerritory = Collision::territory(playerPos, enemyMovPos[earliestEnemyNum]);
				if (isTerritory)
				{
					thunder->SEPlayWave();

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
					ECount -= 1;
				}
			}

			// 雷表示時間
			if (thunderTimer != 0) { thunderTimer--; }

		}

		if (scene == 2)
		{
			if (input->PushKey(DIK_ESCAPE))
			{
				break;
			}
		}
		if (scene == 3)
		{
			if (input->PushKey(DIK_ESCAPE))
			{
				break;
			}
		}

		for (int i = 0; i < enemyNam; i++)
		{
			objEnemyMov[i]->Update();
		}
		input->Update();

		OBJInCoa->Update();
		OBJOutCoaA->Update();
		OBJOutCoaB->Update();
		OBJBack->Update();
		objGround->Update();
		player->Update();
		objChr->Update();
		objThunder->Update();
		camera->Update();
		spriteTitle->Update();
		spriteClear->Update();
		spriteOver->Update();
		for (auto& sprite : sprites)
		{
			sprite->Update();
		}

		// DirectX毎フレーム処理　ここまで
#pragma endregion DirectX毎フレーム処理

#pragma region グラフィックスコマンド

		dxCommon->PreDraw();
		spriteCommon->PreDraw();

		if (scene == 0)
		{
			spriteTitle->Draw();

		}
		if (scene == 1)
		{
			Object3d::PreDraw(dxCommon->GetCmdList());
			OBJBack->Draw();
			objGround->Draw();
			OBJInCoa->Draw();
			OBJOutCoaA->Draw();
			OBJOutCoaB->Draw();

			//objChr->Draw();

			for (int i = 0; i < enemyNam; i++)
			{
				if (enemyFlag[i] == 0) { objEnemyMov[i]->Draw(); }
			}
			/*if (enemyFlag[0] == 0) { objEnemyMov[0]->Draw(); }
			if (enemyFlag[2] == 0) { objEnemyMov[2]->Draw(); }*/
			//player->Draw();
			if (0 < thunderTimer) { objThunder->Draw(); }



		}
		if (scene == 2)
		{
			spriteOver->Draw();

		}
		if (scene == 3)
		{
			spriteClear->Draw();
		}
		Object3d::PostDraw();

		spriteCommon->PreDraw();

		if (scene == 0)
		{
			spriteTitle->Draw();

		}
		if (scene == 1)
		{


			/*	for (auto& sprite : sprites)
				{
				}*/
			spritePlayer->Draw();


		}
		if (scene == 2)
		{

		}
		if (scene == 3)
		{

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
	delete inCoa;
	delete modelChr;
	delete modelThunder;
	delete modelEnemyMovA;
	delete modelEnemyMovB;
	delete modelEnemyMovC;
	delete objChr;
	delete OBJInCoa;
	delete objThunder;

	for (int i = 0; i < enemyNam; i++)
	{
		delete objEnemyMov[i];
	}
	return 0;
}