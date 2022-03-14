﻿#pragma once

#include<vector>

#include "Framework.h"

class GamePlayScene;
class TitleScene;

class MyGame : public Framework
{
public:
	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;



private:
	int counter = 0; // アニメーションの経過時間カウンター

	GamePlayScene* scene_ = nullptr;
};

