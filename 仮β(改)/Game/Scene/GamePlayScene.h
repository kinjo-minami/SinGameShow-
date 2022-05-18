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

	
	//プレイヤー操作
	void PlayerMove();


private:
	
	// Sprite
	Sprite* sprite = nullptr;
	Sprite* spriteClear = nullptr;
	Sprite* spriteOver = nullptr;

	std::vector<Sprite*> sprites;
	std::vector<Sprite*> spritesEnemy;
	std::vector<Sprite*> spritesRader;

	// Object3d
	//std::vector<Object3d*> objStage;
	Model* modelGround = Model::LoadFromOBJ("ground");
	Object3d* objGround = Object3d::Create();

	Model* modelSky = Model::LoadFromOBJ("back");
	Object3d* objSky = Object3d::Create();
	XMFLOAT3 skyPos = {};


	//enemy
	float radius = 500.0f;
	float enemyMove[200] = {};
	XMFLOAT2 sEnemyRe[200] = {};
	float angle[200] = {};
	int enemyNam = 10;

	Model* modelEnemyRat = Model::LoadFromOBJ("rat");
	Object3d* objEnemyMov[200] = {};
	XMFLOAT3 enemyMovPos[200] = {};
	
	
	//カメラ関連
	DebugCamera* camera = nullptr;
	bool dirty = false;
	float angleX = 0;
	float angleY = 0;
	float scaleX = 1.0f / (float)WinApp::window_width;
	float scaleY = 1.0f / (float)WinApp::window_height;
	bool viewDirty = false;
	float distance = 20.0f;
	XMMATRIX matRot = DirectX::XMMatrixIdentity();
	int counter = 0; // アニメーションの経過時間カウンター


	//プレイヤー関連
	Model* modelCloud = Model::LoadFromOBJ("cloud");
	Object3d* objCloud = Object3d::Create();

	XMFLOAT3 cloudPos = objCloud->GetPosition();
	XMFLOAT3 cloudPosRay = objCloud->GetPosition();
	XMFLOAT3 cloudRot = {};
	XMFLOAT3 playerRe = { 1280 - 256 + 8,128,0 };
	XMFLOAT2 raderP = {};
	float rot = 0.0f;


	int gameFlag = 0;

	

	const float PI = 3.1415926f;

};

