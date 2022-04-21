#pragma once
#include"BaseScene.h"
#include"Sprite.h"
#include"Object3d.h"

class TitleScene : public BaseScene {

public:
	TitleScene(SceneManager* sceneManager);

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:
	Sprite* sprite = nullptr;
	std::vector<Sprite*> sprites;
};

