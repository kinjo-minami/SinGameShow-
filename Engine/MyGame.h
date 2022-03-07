#pragma once

#include<vector>

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Audio.h"

#include "Object3d.h"
#include "Model.h"
#include"SpriteCommon.h"
#include"Sprite.h"

class MyGame
{
public:
	void Initialize();

	void Finalize();

	void Update();

	void Draw();

	bool GetGameloopEndReqest() { return gameloopEndReqest_; }

private:
	bool gameloopEndReqest_ = false;
	WinApp* winApp = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	MSG msg{};  // メッセージ


private:
	SpriteCommon* spriteCommon = new SpriteCommon();
	Sprite* sprite = nullptr;
	Model* modelPost = nullptr;
	Model* modelChr = nullptr;
	Object3d* objPost = nullptr;
	Object3d* objChr = nullptr;
	std::vector<Sprite*> sprites;

private:
	int counter = 0; // アニメーションの経過時間カウンター

};

