#include "MyGame.h"

void MyGame::Initialize()
{

#pragma region 描画初期化処理

	Framework::Initialize();

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

#pragma endregion 描画初期化処理
}

void MyGame::Finalize()
{

	for (auto& sprite : sprites)
	{
		delete sprite;
	}
	//delete sprite;
	delete modelPost;
	delete modelChr;
	delete objChr;
	delete objPost;

	Framework::Finalize();
}

void MyGame::Update()
{

#pragma region DirectX毎フレーム処理
	// DirectX毎フレーム処理　ここから
	Framework::Update();


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

	objPost->Update();
	objChr->Update();
	for (auto& sprite : sprites)
	{
		sprite->Update();
	}

	// DirectX毎フレーム処理　ここまで
#pragma endregion DirectX毎フレーム処理
}

void MyGame::Draw()
{
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
