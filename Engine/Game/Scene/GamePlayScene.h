#pragma once
#include"BaseScene.h"
#include"Sprite.h"
#include"Object3d.h"
#include"DebugCamera.h"


class DirectXCommon;

class GamePlayScene : public BaseScene {
public:
	GamePlayScene(SceneManager* sceneManager);
	
	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

public:

	void Create3D_object();

	void Create2D_object();

	void ChangeScene();

	void ClassUpdate();

private:
	Sprite* sprite = nullptr;
	Model* modelPost = nullptr;
	Model* modelChr = nullptr;
	Object3d* objPost = nullptr;
	Object3d* objChr = nullptr;
	DebugCamera* camera;
	std::vector<Sprite*> sprites;
};

