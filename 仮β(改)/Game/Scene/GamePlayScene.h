#pragma once
#include<DirectXMath.h>
#include"BaseScene.h"
#include"Sprite.h"
#include"Object3d.h"
#include"DebugCamera.h"
#include"Collision.h"

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

	//エネミー
	void Enemymove();

	//エネミーとコアのあたり判定
	void EnemyHitCoa();

	//最短距離
	void EnemyPlayerDistance();


	//プレイヤー操作
	void PlayerMove();

	void PlayerAtk();


private:
	
	// Sprite
	Sprite* sprite = nullptr;
	Sprite* spriteClear = nullptr;
	Sprite* spriteOver = nullptr;
	Sprite* spritePlayer = nullptr;
	Sprite* spriteEnemyRe[800] = {};

	std::vector<Sprite*> sprites;
	
	std::vector<Sprite*> spritesRader;

	// Object3d
	//std::vector<Object3d*> objStage;
	Model* modelGround = Model::LoadFromOBJ("ground");
	Object3d* objGround = Object3d::Create();

	Model* modelSky = Model::LoadFromOBJ("back");
	Object3d* objSky = Object3d::Create();
	XMFLOAT3 skyPos = {};


	//エネミー関連
	float radius = 500.0f;
	float enemyMove[800] = {};
	float enemyOriginMove[800] = {};

	XMFLOAT2 sEnemyRe[800] = {};
	float angle[800] = {};
	int enemyNam = 10;
	int enemyWave = 0;
	int enemyFlag[800] = {};
	int enemyCount = 0;
	int enemyDeleteFlag[53] = {};

	Model* modelEnemyRat = Model::LoadFromOBJ("rat");
	Model* modelEnemyFrog = Model::LoadFromOBJ("Frog");
	Model* modelEnemySpider = Model::LoadFromOBJ("spider");
	Model* modelEnemyWani = Model::LoadFromOBJ("wani");
	//Model* modelEnemyRat = Model::LoadFromOBJ("rat");
	Object3d* objEnemyMov[800] = {};
	XMFLOAT3 enemyMovPos[800] = {};
	
	
	//カメラ関連
	DebugCamera* camera = nullptr;
	float angleY = 0;
	float scaleY = 1.0f / (float)WinApp::window_height;
	XMMATRIX matRot = DirectX::XMMatrixIdentity();
	int counter = 0; // アニメーションの経過時間カウンター


	//コア関連
	Model* inCoa = Model::LoadFromOBJ("core_in");
	Model* outCoa1 = Model::LoadFromOBJ("core_out1");
	Model* outCoa2 = Model::LoadFromOBJ("core_out2");
	Object3d* OBJInCoa = Object3d::Create();
	Object3d* OBJOutCoaA = Object3d::Create();
	Object3d* OBJOutCoaB = Object3d::Create();

	XMFLOAT3 CoaRotA = {};
	XMFLOAT3 CoaRotB = {};
	XMFLOAT3 CoaPos = { 0,4,50 };
	int coaHit = 7;


	//プレイヤー関連
	Model* modelCloudThunder = Model::LoadFromOBJ("cloud");
	Model* modelCloudRain = Model::LoadFromOBJ("cloud_rain");
	Model* modelCloudSnow = Model::LoadFromOBJ("cloud_snow");
	Object3d* objCloud = Object3d::Create();

	XMFLOAT3 cloudPos = objCloud->GetPosition();
	XMFLOAT3 cloudRot = {};
	XMFLOAT3 playerRe = { 1280 - 256 + 8,128,0 };
	XMFLOAT2 raderP = {};
	
	//攻撃関連
	int atkFlag = 0;
	//雷
	//int thunderFlag = 0;
	Object3d* objThunder = Object3d::Create();
	Model* modelThunder = Model::LoadFromOBJ("solothunder");

	XMFLOAT3 thunderPos = objThunder->GetPosition();
	int thunderFlag = 0;
	int thunderTimer = 0;

	//雪
	Object3d* objSnow[10] = {};
	Model* modelSnow = Model::LoadFromOBJ("iceHorn");

	XMFLOAT3 snowPos[10] = {};
	int snowFlag[10] = {};
	int snowTimer[10] = {};
	
	//雨
	Object3d* objRain[10] = {};
	Model* modelRain = Model::LoadFromOBJ("puddle");

	XMFLOAT3 rainPos[10] = {};
	int rainFlag[10] = {};
	int rainTimer[10] = {};


	// 最短距離関係
	float earliest[800];
	XMFLOAT3 Earliest;
	int earliestEnemyNum;


	//テリトリー
	Object3d* objPlayerTerritory = Object3d::Create();
	Object3d* objStageTerritory = Object3d::Create();
	Model* modelTerritory = Model::LoadFromOBJ("territory");


	int gameFlag = 0;

	
	int score = 0;

	const float PI = 3.1415926f;

};

