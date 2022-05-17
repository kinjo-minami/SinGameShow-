#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "DirectXCommon.h"
#include "TitleScene.h"

GamePlayScene::GamePlayScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{
}

void GamePlayScene::Initialize() {
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();


	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);

	Object3d::SetCamera(camera);
	gameFlag = 0;
	SpriteLoadTex();

	Create2D_object();

	Create3D_object();

	camera->SetTarget({ 0,0,-30 });
	camera->SetEye({ 0, 0, 0 });

#pragma endregion 描画初期化処理
}

void GamePlayScene::Finalize() {
	for (auto& sprite : sprites)
	{
		delete sprite;
	}
	//delete sprite;

	delete camera;

}

void GamePlayScene::Update() {
	Input* input = Input::GetInstance();

	ClassUpdate();
	if (gameFlag == 0) {


	}
	if (gameFlag == 1) {


	}
	if (gameFlag == 2) {


	}

	if (input->TriggerKey(DIK_RETURN)) {
		ChangeScene();
	}

}

void GamePlayScene::Draw() {

	DirectXCommon* dxcommon_ = DirectXCommon::GetInstance();

	Object3d::PreDraw(dxcommon_->GetCmdList());

	Object3d::PostDraw();

	SpriteCommon::GetInstance()->PreDraw();

	if (gameFlag == 0) {

		for (auto& sprite : spritesRader)
		{

			sprite->Draw();
		}
		for (auto& sprite : spritesEnemy)
		{

			sprite->Draw();
		}

	}
	if (gameFlag == 1) {

		spriteClear->Draw();

	}
	if (gameFlag == 2) {

		spriteOver->Draw();

	}

	
	//sprite->Draw();
}

void GamePlayScene::Create3D_object() {


}

void GamePlayScene::Create2D_object() {

	/*sprite->Create(5, { 0,0 }, false, false);
	sprites.push_back(sprite);
	sprite->SetPosition({ 500,300,0 });*/

	sprite = Sprite::Create(0, { 0,0 }, false, false);
	spritesRader.push_back(sprite);
	spriteClear = Sprite::Create(1, { 0,0 }, false, false);
	//sprites.push_back(sprite);
	spriteOver = Sprite::Create(2, { 0,0 }, false, false);
	//sprites.push_back(sprite);

	sprite = Sprite::Create(3, { 0,0 }, false, false);
	sprite->SetPosition({ 1280 - 256,0,0 });
	spritesRader.push_back(sprite);

	sprite = Sprite::Create(4, { 0,0 }, false, false);
	sprite->SetPosition({ 1280 - 256 - 120,0,0 });
	spritesRader.push_back(sprite);

	sprite = Sprite::Create(5, { 0,0 }, false, false);
	sprite->SetPosition({ 1280 - 256,0,0 });
	spritesRader.push_back(sprite);

	for (int i = 0; i < enemyNam; i++)
	{
		sprite = Sprite::Create(6, { 0,0 }, false, false);
		int ran = rand() % 360 + 1;
		angle[i] = (float)ran;
		sEnemyRe[i] = { 1280 - 256,0 };
		sEnemyRe[i].x += cos((angle[i] * PI) / 180) * 128;
		sEnemyRe[i].y += sin((angle[i] * PI) / 180) * 128;
		sprite->SetPosition({ sEnemyRe[i].x,sEnemyRe[i].y,0 });
		sprite->Update();
		spritesEnemy.push_back(sprite);
		enemyMove[i] = (float)(rand() % 3 + 1);
		enemyMove[i] = enemyMove[i] / 10.0f;
	}

}

void GamePlayScene::ChangeScene() {

	BaseScene* scene = new TitleScene(sceneManager_);
	sceneManager_->SetNextScene(scene);
}

void GamePlayScene::ClassUpdate() {


	camera->Update();
	for (auto& sprite : spritesRader)
	{
		sprite->Update();
	}
}

void GamePlayScene::SpriteLoadTex() {
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->LoadTexture(0, L"Resources/hud.png");
	//spriteCommon->LoadTexture(5, L"Resources/title2.png");
	spriteCommon->LoadTexture(1, L"Resources/gameClear.png");
	spriteCommon->LoadTexture(2, L"Resources/gameover.png");
	spriteCommon->LoadTexture(3, L"Resources/reader.png");
	spriteCommon->LoadTexture(4, L"Resources/playerRe.png");
	spriteCommon->LoadTexture(5, L"Resources/coraRe.png");
	spriteCommon->LoadTexture(6, L"Resources/enemyRe.png");

	//spriteCommon->LoadTexture(12, L"Resources/rain2.png");
}

void GamePlayScene::CameraCreateSet() {
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);

	Object3d::SetCamera(camera);

	camera->SetTarget({ 0,20,0 });
	camera->SetDistance(100.0f);
	camera->SetEye({ 0, 0, 0 });
}
