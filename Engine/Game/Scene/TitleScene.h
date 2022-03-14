#pragma once
#include"Sprite.h"
#include"Object3d.h"

class TitleScene {

public:
	void Initialize();

	void Finalize();

	void Update();

	void Draw();

private:
	Sprite* sprite = nullptr;
	std::vector<Sprite*> sprites;
};

