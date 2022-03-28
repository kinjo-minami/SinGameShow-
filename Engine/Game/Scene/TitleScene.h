#pragma once
#include"IScene.h"
#include"Sprite.h"
#include"Object3d.h"

class TitleScene : public IScene {

public:
	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:
	Sprite* sprite = nullptr;
	std::vector<Sprite*> sprites;
};

