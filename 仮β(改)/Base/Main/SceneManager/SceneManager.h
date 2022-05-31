#pragma once

#include "BaseScene.h"

//Scene管理

class SceneManager {
public:
	~SceneManager();

	void Update();

	void Draw();
public:
	//次シーン予約
	void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }

	void SetBestScore(float bestScore) { bestScore_ = bestScore; }

	float GetBestScore() { return bestScore_; }
private:

	float bestScore_ = 0;
	//今
	BaseScene* scene_ = nullptr;
	//次
	BaseScene* nextScene_ = nullptr;
};

