#include "TitleScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "DirectXCommon.h"
#include "GamePlayScene.h"

TitleScene::TitleScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{
}

void TitleScene::Initialize() {
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->LoadTexture(0, L"Resources/title2.png");
	spriteCommon->LoadTexture(1, L"Resources/rain2.png");
	title =Sprite::Create(0, { 0,0 }, false, false);
	rain = Sprite::Create(1, { 0,0 }, false, false);
}

void TitleScene::Finalize() {
	delete title;
	delete rain;
}

void TitleScene::Update() {
	Input* input = Input::GetInstance();

	// 雨の動き
	titleRainPos.x -= 4;
	titleRainPos.y += 4;

	if (titleRainPos.x == -1280) {
		titleRainPos.x = 0;
	}
	if (titleRainPos.y == 0) {
		titleRainPos.y = -1280;
	}

	rain->SetPosition(titleRainPos);

	if (input->TriggerMouseLeft())
	{
		BaseScene* scene = new GamePlayScene(sceneManager_);
		sceneManager_->SetNextScene(scene);
	}

	title->Update();
	rain->Update();
}

void TitleScene::Draw() {
	SpriteCommon::GetInstance()->PreDraw();
	rain->Draw();
	title->Draw();
}
