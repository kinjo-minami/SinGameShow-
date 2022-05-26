#include "MyGame.h"
#include "GamePlayScene.h"
#include "TitleScene.h"

void MyGame::Initialize() {

	Framework::Initialize();
	BaseScene* scene = new TitleScene(sceneManager_);
	sceneManager_->SetNextScene(scene);
}

void MyGame::Finalize() {

	Framework::Finalize();
}

void MyGame::Update() {

	Framework::Update();
}

void MyGame::Draw() {
	Framework::Draw();
}
