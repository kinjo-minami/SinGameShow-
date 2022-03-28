#pragma once
#include "Iscene.h"
#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include"SpriteCommon.h"

#include "Object3d.h"
#include "Model.h"
#include"Sprite.h"

class Framework
{
public:

	void Run();

	virtual void Initialize();

	virtual void Finalize();

	virtual void Update();
	
	virtual void Draw();

	virtual bool GetGameloopEndReqest() { return gameloopEndReqest_; }


protected:
	bool gameloopEndReqest_ = false;
	WinApp* winApp_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	SpriteCommon* spriteCommon_ = nullptr;
	Audio* audio_ = nullptr;
	IScene* scene_ = nullptr;
};

