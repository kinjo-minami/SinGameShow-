#pragma once

class SceneManager;

//�V�[�����
class BaseScene {
public:
	BaseScene(SceneManager* sceneManager);

	virtual void Initialize() = 0;

	virtual void Finalize() = 0;

	virtual void Update() = 0;

	virtual void Draw() = 0;

protected:
	SceneManager* sceneManager_ = nullptr;

};

