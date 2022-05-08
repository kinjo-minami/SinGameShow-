
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
#include "Player.h"

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
	DebugCamera* cameraRay = nullptr;
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height, input);
	Object3d::SetCamera(camera);

	// カメラ注視点をセット
	camera->SetTarget({ 0, 1, -400 });
	camera->SetDistance(3.0f);

#pragma endregion DirectX初期化処理

#pragma region 描画初期化処理

	SpriteCommon* spriteCommon = new SpriteCommon();
	SpriteCommon* spriteCommon2 = new SpriteCommon();
	spriteCommon->initialize(dxCommon->GetDev(), dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);
	spriteCommon2->initialize(dxCommon->GetDev(), dxCommon->GetCmdList(), winApp->window_width, winApp->window_height);

	spriteCommon->LoadTexture(0, L"Resources/texture.png");
	spriteCommon->LoadTexture(1, L"Resources/house.png");
	spriteCommon->LoadTexture(2, L"Resources/player.png");
	spriteCommon->LoadTexture(3, L"Resources/hp_bar.png");
	spriteCommon->LoadTexture(4, L"Resources/mouse.png");
	spriteCommon->LoadTexture(5, L"Resources/title.png");
	spriteCommon->LoadTexture(6, L"Resources/gameClear.png");
	spriteCommon->LoadTexture(7, L"Resources/gameover.png");
	spriteCommon->LoadTexture(8, L"Resources/coraRe.png");
	spriteCommon->LoadTexture(9, L"Resources/enemyRe.png");
	spriteCommon->LoadTexture(10, L"Resources/playerRe.png");
	spriteCommon->LoadTexture(11, L"Resources/reader.png");

	std::vector<Sprite*> sprites;
	/*Sprite* sprite = Sprite::Create(spriteCommon, 0);
	sprites.push_back(sprite);
	sprite->SetPosition({ 500,300,0 });*/
	Sprite* spritePlayer = Sprite::Create(spriteCommon, 2, { 0,0 }, false, false);
	Sprite* spriteTitle = Sprite::Create(spriteCommon, 5, { 0,0 }, false, false);
	Sprite* spriteCoraRe = Sprite::Create(spriteCommon, 8, { 0,0 }, false, false);

	spriteCoraRe->SetPosition({ 1280 - 256,0,0 });
	spriteCoraRe->Update();
	Sprite* spriteEnemyRe[10] = {};

	for (int i = 0; i < 10; i++)
	{
		spriteEnemyRe[i] = Sprite::Create(spriteCommon, 9, { 0,0 }, false, false);
	}


	Sprite* spritePlayerRe = Sprite::Create(spriteCommon, 10, { 0,0 }, false, false);
	spritePlayerRe->SetPosition({ 1280 - 256 - 120,0,0 });
	spritePlayerRe->Update();
	Sprite* spriteReader = Sprite::Create(spriteCommon, 11, { 0,0 }, false, false);
	spriteReader->SetPosition({ 1280 - 256,0,0 });
	spriteReader->Update();
	spritePlayer->SetSize({ 1280 ,720 });
	spritePlayer->TransferVertexBuffer();
	spriteTitle->SetSize({ 1280 ,720 });

	//spriteTitle->Update();
	spriteTitle->TransferVertexBuffer();
	Sprite* spriteMouse = Sprite::Create(spriteCommon, 4, { 0,0 }, false, false);
	Sprite* spriteClear = Sprite::Create(spriteCommon, 6, { 0,0 }, false, false);
	Sprite* spriteOver = Sprite::Create(spriteCommon, 7, { 0,0 }, false, false);

	spriteOver->SetSize({ 1280 ,720 });
	spriteClear->SetSize({ 1280 ,720 });
	spriteOver->TransferVertexBuffer();
	spriteClear->TransferVertexBuffer();

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
	Model* modelCloud = Model::LoadFromOBJ("cloud");

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
	Object3d* objCloud = Object3d::Create();

	player->SetModel(modelChr);
	OBJInCoa->SetModel(inCoa);
	OBJOutCoaA->SetModel(outCoa1);
	OBJOutCoaB->SetModel(outCoa2);
	OBJBack->SetModel(modelBack);
	objGround->SetModel(modelGround);

	objChr->SetModel(modelChr);
	objThunder->SetModel(modelThunder);
	objCloud->SetModel(modelCloud);

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
	//エネミー
	float enemyMove[10] = {};
	XMFLOAT2 sEnemyRe[10] = {};

	for (int i = 0; i < enemyNam; i++)
	{
		sEnemyRe[i] = { 1280 - 256,0 };
		sEnemyRe[i].x += cos((angle[i] * PI) / 180) * 128;
		sEnemyRe[i].y += sin((angle[i] * PI) / 180) * 128;
		spriteEnemyRe[i]->SetPosition({ sEnemyRe[i].x,sEnemyRe[i].y,0 });
		spriteEnemyRe[i]->Update();
		enemyMove[i] = (float)(rand() % 3 + 1);
		enemyMove[i] = enemyMove[i] / 10.0f;
	}
	player->SetPosition({ 0.0f,0.0f,0.0f });
	objThunder->SetPosition({ 0.0f,100.0f,0.0f });
	camera->SetEye({ 0, 0, 0 });
	cameraRay = camera;
	/*objPost->Update();
	objChr->Update();*/
	OBJBack->SetScale({ 6.0f,6.0f,6.0f });
#pragma endregion 描画初期化処理

	int counter = 0; // アニメーションの経過時間カウンター

	int scene = 0;

	// カメラ関係
	/*float angleY = 0;
	float scaleY = 1.0f / (float)WinApp::window_height;
	float distance = 20.0f;
	XMMATRIX matRot = DirectX::XMMatrixIdentity();*/

	//スカイドーム
	XMFLOAT3 skyPos = OBJBack->GetPosition();

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

	// 雲関係
	XMFLOAT3 cloudPos = objCloud->GetPosition();
	XMFLOAT3 cloudPosRay = objCloud->GetPosition();

	XMFLOAT3 cloudRot = objCloud->GetRotation();
	XMFLOAT3 playerRe = { 1280 - 256 + 8,128,0 };
	XMFLOAT2 raderP = {};
	float rot = 0.0f;
	spritePlayerRe->SetAnchorpoint({ 0.5f,0.5f });

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
			//rot += 0.1f;
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

			bool skyHit = Collision::Virtualitys(cloudPos, skyPos);
			bool UnSkyHit = Collision::UnVirtualitys(cloudPos, skyPos);

			Player::PlayerMouse(input);
			Player::PlayerMove(camera, input);
			Player::PlayerVector(camera, objCloud, cloudPos, cloudRot);

			//// マウスの入力を取得
			//Input::MouseMove mouseMove = input->GetMouseMove();
			//float dy = mouseMove.lX * scaleY;
			//angleY = -dy * XM_PI;

			//// ボタンが押されていたらカメラを並行移動させる
			//if (input->PushKey(DIK_D))
			//{
			//	XMVECTOR move = { 1.0f, 0, 0, 0 };
			//	move = XMVector3Transform(move, matRot);
			//	camera->MoveVector(move);
			//}
			//if (input->PushKey(DIK_A))
			//{
			//	XMVECTOR move = { -1.0f, 0, 0, 0 };
			//	move = XMVector3Transform(move, matRot);
			//	camera->MoveVector(move);
			//}
			//if (input->PushKey(DIK_W))
			//{
			//	XMVECTOR move = { 0, 0, 1.0f, 0 };
			//	move = XMVector3Transform(move, matRot);
			//	camera->MoveVector(move);
			//}
			//if (input->PushKey(DIK_S))
			//{
			//	XMVECTOR move = { 0, 0, -1.0f, 0 };
			//	move = XMVector3Transform(move, matRot);
			//	camera->MoveVector(move);
			//}

			//// 追加回転分の回転行列を生成
			//XMMATRIX matRotNew = XMMatrixIdentity();
			//matRotNew *= XMMatrixRotationY(-angleY);
			//// 累積の回転行列を合成
			//// ※回転行列を累積していくと、誤差でスケーリングがかかる危険がある為
			//// クォータニオンを使用する方が望ましい
			//matRot = matRotNew * matRot;

			//// 注視点から視点へのベクトルと、上方向ベクトル
			//XMVECTOR vTargetEye = { 0.0f, 0.0f, -distance, 1.0f };
			//XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };

			//// ベクトルを回転
			//vTargetEye = XMVector3Transform(vTargetEye, matRot);
			//vUp = XMVector3Transform(vUp, matRot);

			//// 長さ
			//float length = 0.0f;

			//XMFLOAT3 target1 = camera->GetTarget();
			//camera->SetEye({ target1.x + vTargetEye.m128_f32[0], target1.y + vTargetEye.m128_f32[1], target1.z + vTargetEye.m128_f32[2] });
			//camera->SetUp({ vUp.m128_f32[0], vUp.m128_f32[1], vUp.m128_f32[2] });

			//// 注視点からずらした位置に視点座標を決定
			//XMFLOAT3 target2 = camera->GetTarget();
			//XMFLOAT3 eye = camera->GetEye();

			//XMFLOAT3 fTargetEye = { 0.0f, 0.0f, 0.0f };

			//// 大きさ計算
			//length = sqrtf(pow(target2.x - eye.x, 2) + pow(target2.y - eye.y, 2) + pow(target2.z - eye.z, 2));
			//fTargetEye.x = eye.x - target2.x;
			//fTargetEye.y = eye.y - target2.y;
			//fTargetEye.z = eye.z - target2.z;

			//fTargetEye.x /= length;
			//fTargetEye.y /= length;
			//fTargetEye.z /= length;

			//fTargetEye.x *= 17;
			//fTargetEye.y *= 17;
			//fTargetEye.z *= 17;

			//objCloud->SetScale({ 1.0f, 1.0f, 1.0f });

			//cloudPos = { target2.x + fTargetEye.x, target2.y + fTargetEye.y - 1.5f, target2.z + fTargetEye.z };
			//cloudPosRay = { target2.x + fTargetEye.x, target2.y + fTargetEye.y - 1.5f, target2.z + fTargetEye.z };

			//objCloud->SetPosition(cloudPos);

			//cloudRot.y = atan2f(-fTargetEye.x, -fTargetEye.z);
			//cloudRot.y *= 180 / PI;
			//objCloud->SetRotation({ 0.0f, cloudRot.y, 0.0f });

			spritePlayerRe->SetPosition(playerRe);
			spritePlayerRe->SetRotation(cloudRot.y + 90);

			if (input->PushKey(DIK_ESCAPE))
			{
				break;
			}

			if (input->PushKey(DIK_D))
			{
				if (skyHit)
				{
					playerRe.y += sin(((cloudRot.y + 90) * PI) / 180) * (1.0f / 3.90625f);
					playerRe.x += cos(((cloudRot.y + 90) * PI) / 180) * (1.0f / 3.90625f);
				}
			}
			if (input->PushKey(DIK_A))
			{
				if (skyHit)
				{
					playerRe.y -= sin(((cloudRot.y + 90) * PI) / 180) * (1.0f / 3.90625f);
					playerRe.x -= cos(((cloudRot.y + 90) * PI) / 180) * (1.0f / 3.90625f);
				}

			}
			if (input->PushKey(DIK_W))
			{
				if (skyHit)
				{
					playerRe.y += sin((cloudRot.y * PI) / 180) * (1.0f / 3.90625f);
					playerRe.x += cos((cloudRot.y * PI) / 180) * (1.0f / 3.90625f);
				}

			}
			if (input->PushKey(DIK_S))
			{
				if (skyHit)
				{
					playerRe.y -= sin((cloudRot.y * PI) / 180) * (1.0f / 3.90625f);
					playerRe.x -= cos((cloudRot.y * PI) / 180) * (1.0f / 3.90625f);
				}

			}

			//エネミー移動
			for (int i = 0; i < enemyNam; i++)
			{
				XMFLOAT3 vel = {};
				vel.x = sin((angle[i] * PI) / 180) * enemyMove[i];
				vel.y = 0.0f;
				vel.z = cos((angle[i] * PI) / 180) * enemyMove[i];
				enemyMovPos[i].x -= vel.x;
				enemyMovPos[i].y -= vel.y;
				enemyMovPos[i].z -= vel.z;
				sEnemyRe[i].x -= cos((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				sEnemyRe[i].y -= sin((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				spriteEnemyRe[i]->SetPosition({ sEnemyRe[i].x,sEnemyRe[i].y,0 });
				spriteEnemyRe[i]->Update();
				objEnemyMov[i]->SetPosition(enemyMovPos[i]);
				objEnemyMov[i]->Update();
			}

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

#pragma region enemy hit 
			for (int i = 0; i < enemyNam; i++)
			{
				if (enemyFlag[i] == 0)
				{
					Earliest.x = cloudPos.x - enemyMovPos[i].x;
					Earliest.y = cloudPos.y - enemyMovPos[i].y;
					Earliest.z = cloudPos.z - enemyMovPos[i].z;
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
						else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
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
				bool isTerritory = Collision::territory(cloudPos, enemyMovPos[earliestEnemyNum]);
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
#pragma endregion enemy hit
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

		camera->Update();
		cameraRay->Update();
		OBJInCoa->Update();
		OBJOutCoaA->Update();
		OBJOutCoaB->Update();
		OBJBack->Update();
		objGround->Update();
		player->Update();
		objChr->Update();
		objThunder->Update();
		objCloud->Update();

		spriteTitle->Update();
		spriteClear->Update();
		spriteOver->Update();
		spritePlayerRe->Update();

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
			objCloud->Draw();
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
				//spritePlayer->Draw();
			spriteReader->Draw();
			spriteCoraRe->Draw();
			spritePlayerRe->Draw();

			for (int i = 0; i < enemyNam; i++)
			{
				if (enemyFlag[i] == 0) { spriteEnemyRe[i]->Draw(); }
			}

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
	delete spriteCommon2;
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
	delete modelCloud;
	delete objChr;
	delete OBJInCoa;
	delete objThunder;
	delete objCloud;

	for (int i = 0; i < enemyNam; i++)
	{
		delete objEnemyMov[i];
	}
	return 0;
}