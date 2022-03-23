
#include<vector>

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"

#include "Object3d.h"
#include "Model.h"
#include"SpriteCommon.h"
#include"Sprite.h"
#include"DebugCamera.h"

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
	camera->SetTarget({ 0, 1, 0 });
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

	Object3d* objPost = Object3d::Create();
	Object3d* objChr = Object3d::Create();

	objPost->SetModel(modelPost);
	objChr->SetModel(modelChr);

	objPost->SetPosition({ 0,0,50 });
	objChr->SetPosition({ 0,-25,-75 });

	camera->SetTarget({ 0,0,-100 });
	camera->SetEye({ 0, 0, 0 });

	/*objPost->Update();
	objChr->Update();*/

#pragma endregion 描画初期化処理

	int counter = 0; // アニメーションの経過時間カウンター

	bool dirty = false;
	float angleX = 0;
	float angleY = 0;
	float scaleX = 1.0f / (float)WinApp::window_width;
	float scaleY = 1.0f / (float)WinApp::window_height;
	bool viewDirty = false;
	float distance = 20.0f;
	XMMATRIX matRot = DirectX::XMMatrixIdentity();

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
		objChr->SetModel(modelChr);
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

		//XMFLOAT3 ChrPos = objChr->GetPosition();

		// マウスの入力を取得
		Input::MouseMove mouseMove = input->GetMouseMove();
		float dy = mouseMove.lX * scaleY;
		angleY = -dy * XM_PI;

		// ボタンが押されていたらカメラを並行移動させる
		if (input->PushKey(DIK_D))
		{
			XMVECTOR move = { 1.0f, 0, 0, 0 };
			move = XMVector3Transform(move, matRot);
			camera->MoveVector(move);
		}
		if (input->PushKey(DIK_A))
		{
			XMVECTOR move = { -1.0f, 0, 0, 0 };
			move = XMVector3Transform(move, matRot);
			camera->MoveVector(move);
		}
		if (input->PushKey(DIK_W))
		{
			XMVECTOR move = { 0, 0, 1.0f, 0 };
			move = XMVector3Transform(move, matRot);
			camera->MoveVector(move);
		}
		if (input->PushKey(DIK_S))
		{
			XMVECTOR move = { 0, 0, -1.0f, 0 };
			move = XMVector3Transform(move, matRot);
			camera->MoveVector(move);
		}

		dirty = true;

		if (dirty || viewDirty) {
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

		//objChr->SetPosition(ChrPos);

		objPost->Update();
		objChr->Update();
		camera->Update();
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
			sprite->Draw();
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