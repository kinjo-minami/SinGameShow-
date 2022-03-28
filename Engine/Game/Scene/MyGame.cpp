#include "MyGame.h"
#include "GamePlayScene.h"
#include "TitleScene.h"

void MyGame::Initialize() {

	Framework::Initialize();
	scene_ = new GamePlayScene();
	scene_->Initialize();
}

void MyGame::Finalize() {

	scene_->Finalize();
	
	Framework::Finalize();
}

void MyGame::Update() {

	Framework::Update();
}

void MyGame::Draw() {
	Framework::Draw();
}
