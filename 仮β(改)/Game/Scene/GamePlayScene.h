#pragma once
#include<DirectXMath.h>
#include"BaseScene.h"
#include"Sprite.h"
#include"Object3d.h"
#include"DebugCamera.h"


class DirectXCommon;

class GamePlayScene : public BaseScene {
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
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
	void SpriteLoadTex();
	void CameraCreateSet();
private:
	
	Sprite* sprite = nullptr;
	Sprite* spriteClear = nullptr;
	Sprite* spriteOver = nullptr;

	std::vector<Sprite*> sprites;
	std::vector<Sprite*> spritesEnemy;
	std::vector<Sprite*> spritesRader;

	DebugCamera* camera = nullptr;

	int gameFlag = 0;

	float enemyMove[200] = {};
	XMFLOAT2 sEnemyRe[200] = {};
	float angle[200] = {};
	int enemyNam = 10;
	const float PI = 3.1415926f;

};

