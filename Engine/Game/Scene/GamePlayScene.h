#pragma once
#include"IScene.h"
#include"Sprite.h"
#include"Object3d.h"

class GamePlayScene : public IScene {
public:
	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:
	Sprite* sprite = nullptr;
	Model* modelPost = nullptr;
	Model* modelChr = nullptr;
	Object3d* objPost = nullptr;
	Object3d* objChr = nullptr;
	std::vector<Sprite*> sprites;
};

