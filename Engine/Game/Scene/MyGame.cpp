#include "MyGame.h"

void MyGame::Initialize() {

#pragma region 描画初期化処理

	Framework::Initialize();

	
}

void MyGame::Finalize() {

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

void MyGame::Update() {

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
	if (input_->TriggerKey(DIK_0)) // 数字の0キーが押されていたら
	{
		OutputDebugStringA("Hit 0\n");  // 出力ウィンドウに「Hit 0」と表示
	}

	float clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // 青っぽい色

	if (input_->PushKey(DIK_SPACE))     // スペースキーが押されていたら
	{
		// 画面クリアカラーの数値を書き換える
		clearColor[1] = 1.0f;
		objPost->SetModel(modelChr);
		objChr->SetModel(modelChr);
	}

	// 座標操作
	if (input_->PushKey(DIK_UP) || input_->PushKey(DIK_DOWN) || input_->PushKey(DIK_RIGHT) || input_->PushKey(DIK_LEFT))
	{

	}


	if (input_->PushKey(DIK_D) || input_->PushKey(DIK_A))
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

void MyGame::Draw() {
#pragma region グラフィックスコマンド

	dxCommon_->PreDraw();

	Object3d::PreDraw(dxCommon_->GetCmdList());
	objPost->Draw();
	objChr->Draw();
	Object3d::PostDraw();

	spriteCommon_->PreDraw();
	for (auto& sprite : sprites)
	{
		sprite->Draw();
	}

	// ４．描画コマンドここまで
	dxCommon_->PostDraw();

}
