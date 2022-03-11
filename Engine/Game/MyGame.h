#pragma once

#include<vector>

#include "Framework.h"

class MyGame : public Framework
{
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

private:
	int counter = 0; // アニメーションの経過時間カウンター

};

