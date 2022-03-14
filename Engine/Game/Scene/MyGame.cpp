#include "MyGame.h"
#include "GamePlayScene.h"

void MyGame::Initialize() {

#pragma region 描画初期化処理

	Framework::Initialize();
	scene_ = new GamePlayScene();
	scene_->Initialize();
	
}

void MyGame::Finalize() {

	scene_->Finalize();
	
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
	
}

void MyGame::Draw() {
#pragma region グラフィックスコマンド

	dxCommon_->PreDraw();

	scene_->Draw();

	// ４．描画コマンドここまで
	dxCommon_->PostDraw();

}
