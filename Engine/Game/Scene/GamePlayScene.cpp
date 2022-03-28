#include "GamePlayScene.h"
#include "Input.h"
#include "DirectXCommon.h"

void GamePlayScene::Initialize() {
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->LoadTexture(0, L"Resources/texture.png");
	spriteCommon->LoadTexture(1, L"Resources/house.png");

	/*Sprite* sprite = Sprite::Create(spriteCommon, 0);
	sprites.push_back(sprite);
	sprite->SetPosition({ 500,300,0 });*/

	for (int i = 0; i < 20; i++)
	{
		int texNum = rand() % 2;

		sprite = Sprite::Create(texNum, { 0,0 }, false, false);

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

#pragma endregion 描画初期化処理
}

void GamePlayScene::Finalize() {
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

void GamePlayScene::Update() {
	Input* input = Input::GetInstance();
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

	objPost->Update();
	objChr->Update();
	for (auto& sprite : sprites)
	{
		sprite->Update();
	}

	// DirectX毎フレーム処理　ここまで
#pragma endregion DirectX毎フレーム処理
}

void GamePlayScene::Draw() {
	Object3d::PreDraw();
	objPost->Draw();
	objChr->Draw();

	SpriteCommon::GetInstance()->PreDraw();
	for (auto& sprite : sprites)
	{
		sprite->Draw();
	}
}
