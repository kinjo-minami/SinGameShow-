#pragma once
#include"BaseScene.h"
#include"Sprite.h"
#include"Object3d.h"
#include"Audio.h"
class TitleScene : public BaseScene {

public:
	TitleScene(SceneManager* sceneManager);

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:
	Sprite* title = nullptr;
	Sprite* rain = nullptr;
	Audio* bgm = new Audio;;
	char bgmFlag = 0;
	//std::vector<Sprite*> sprites;
	// 変数宣言
	DirectX::XMFLOAT3 titleRainPos = { 0, -1280, 0 };
};

