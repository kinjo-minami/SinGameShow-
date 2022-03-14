#pragma once
#include"Sprite.h"
#include"Object3d.h"

class GamePlayScene
{
public:
	void Initialize();

	void Update();

	void Draw();

private:
	Sprite* sprite = nullptr;
	Model* modelPost = nullptr;
	Model* modelChr = nullptr;
	Object3d* objPost = nullptr;
	Object3d* objChr = nullptr;
	std::vector<Sprite*> sprites;
};

