#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "DirectXCommon.h"
#include "TitleScene.h"
#include"Collision.h"
using namespace DirectX;
using XMFLOAT2 = DirectX::XMFLOAT2;
using XMFLOAT3 = DirectX::XMFLOAT3;
using XMFLOAT4 = DirectX::XMFLOAT4;
using XMVECTOR = DirectX::XMVECTOR;
using XMMATRIX = DirectX::XMMATRIX;



GamePlayScene::GamePlayScene(SceneManager* sceneManager)
	:BaseScene(sceneManager)
{
}

void GamePlayScene::Initialize() {
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);

	Object3d::SetCamera(camera);
	gameFlag = 0;
	SpriteLoadTex();

	Create2D_object();

	Create3D_object();

	camera->SetTarget({ 0, 1, -400 });
	camera->SetDistance(3.0f);
	camera->SetEye({ 0, 0, 0 });

#pragma endregion 描画初期化処理
}

void GamePlayScene::Finalize() {
	for (auto& sprite : sprites) {
		delete sprite;
	}

	for (int i = 0; i < enemyNam; i++) {
		delete objEnemyMov[i];
	}


	delete camera;
}

void GamePlayScene::Create3D_object() {

	//ステージ
	objGround->SetModel(modelGround);
	objGround->SetPosition({ 0,-10,50 });

	objSky->SetModel(modelSky);
	objSky->SetPosition({ 0,0,50 });
	objSky->SetScale({ 6.0f,6.0f,6.0f });
	skyPos = objSky->GetPosition();

	//プレイヤー
	objCloud->SetModel(modelCloud);
	cloudPos = objCloud->GetPosition();
	cloudPosRay = objCloud->GetPosition();
	cloudRot = objCloud->GetRotation();

	//enemy複製
	for (int i = 0; i < 200; i++) {
		enemyMovPos[i] = { 0,4,50 };
	}
	for (int i = 0; i < enemyNam; i++) {
		objEnemyMov[i] = Object3d::Create();
		objEnemyMov[i]->SetModel(modelEnemyRat);
		XMFLOAT3 vel{};
		//angle[i] = 60.0f;
		const float rnd_acc = 0.0000f;
		//float radius = (float)(rand() % 600);
		vel.x = sin((angle[i] * PI) / 180) * radius;
		vel.y = 0.0f;
		vel.z = cos((angle[i] * PI) / 180) * radius;
		enemyMovPos[i].x += vel.x;
		enemyMovPos[i].y += vel.y;
		enemyMovPos[i].z += vel.z;

		objEnemyMov[i]->SetRotation({ 0.0f,angle[i],0.0f });
		objEnemyMov[i]->SetPosition(enemyMovPos[i]);
		objEnemyMov[i]->Update();
	}



}

void GamePlayScene::Update() {
	Input* input = Input::GetInstance();

	ClassUpdate();
	if (gameFlag == 0) {
		PlayerMove();


	}
	if (gameFlag == 1) {


	}
	if (gameFlag == 2) {


	}

	if (input->TriggerKey(DIK_RETURN)) {
		ChangeScene();

	}
}

void GamePlayScene::PlayerMove() {
	Input* input = Input::GetInstance();
	const int cycle = 540; // 繰り返しの周期
	counter++;
	counter %= cycle; // 周期を超えたら0に戻る
	float scale = (float)counter / cycle; // [0,1]の数値

	scale *= 360.0f;
	// マウスの入力を取得
	Input::MouseMove mouseMove = input->GetMouseMove();
	float dy = mouseMove.lX * scaleY;
	angleY = -dy * XM_PI;
	XMFLOAT3 oldCamera = camera->GetTarget();
	XMFLOAT3 oldCloudPos = cloudPos;
	XMFLOAT3 oldCloudPosRay = cloudPosRay;
	XMFLOAT3 oldCameraEye = camera->GetEye();

	if (input->PushKey(DIK_D))
	{
		XMVECTOR move = { 1.0f, 0, 0, 0 };
		move = XMVector3Transform(move, matRot);
		camera->MoveVector(move);
		//cameraRay = camera;

	}
	if (input->PushKey(DIK_A))
	{
		XMVECTOR move = { -1.0f, 0, 0, 0 };
		move = XMVector3Transform(move, matRot);
		camera->MoveVector(move);
		//cameraRay = camera;


	}
	if (input->PushKey(DIK_W))
	{
		XMVECTOR move = { 0, 0, 1.0f, 0 };
		move = XMVector3Transform(move, matRot);
		camera->MoveVector(move);
		//cameraRay = camera;


	}
	if (input->PushKey(DIK_S))
	{
		XMVECTOR move = { 0, 0, -1.0f, 0 };
		move = XMVector3Transform(move, matRot);
		camera->MoveVector(move);
		//cameraRay = camera;

	}

	XMMATRIX matRotNew = XMMatrixIdentity();
	matRotNew *= XMMatrixRotationY(-angleY);
	// 累積の回転行列を合成
	// ※回転行列を累積していくと、誤差でスケーリングがかかる危険がある為
	// クォータニオンを使用する方が望ましい
	matRot = matRotNew * matRot;

	// 注視点から視点へのベクトルと、上方向ベクトル
	XMVECTOR vTargetEye = { 0.0f, 0.0f, -distance, 1.0f };
	XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };

	// ベクトルを回転
	vTargetEye = XMVector3Transform(vTargetEye, matRot);
	vUp = XMVector3Transform(vUp, matRot);

	// 長さ
	float length = 0.0f;

	XMFLOAT3 target1 = camera->GetTarget();
	camera->SetEye({ target1.x + vTargetEye.m128_f32[0], target1.y + vTargetEye.m128_f32[1], target1.z + vTargetEye.m128_f32[2] });
	camera->SetUp({ vUp.m128_f32[0], vUp.m128_f32[1], vUp.m128_f32[2] });

	// 注視点からずらした位置に視点座標を決定
	XMFLOAT3 target2 = camera->GetTarget();
	XMFLOAT3 eye = camera->GetEye();

	XMFLOAT3 fTargetEye = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 fTargetEye2 = { 0.0f, 0.0f, 0.0f };

	// 大きさ計算
	length = sqrtf(pow(target2.x - eye.x, 2) + pow(target2.y - eye.y, 2) + pow(target2.z - eye.z, 2));
	fTargetEye.x = eye.x - target2.x;
	fTargetEye.y = eye.y - target2.y;
	fTargetEye.z = eye.z - target2.z;

	fTargetEye.x /= length;
	fTargetEye.y /= length;
	fTargetEye.z /= length;

	fTargetEye2 = fTargetEye;

	fTargetEye2.x *= 14;
	fTargetEye2.y *= 14;
	fTargetEye2.z *= 14;

	fTargetEye.x *= 17;
	fTargetEye.y *= 17;
	fTargetEye.z *= 17;

	objCloud->SetScale({ 1.0f, 1.0f, 1.0f });

	cloudPos = { target2.x + fTargetEye.x, target2.y + fTargetEye.y - 1.5f, target2.z + fTargetEye.z };
	cloudPosRay = { target2.x + fTargetEye2.x, target2.y + fTargetEye2.y - 1.5f, target2.z + fTargetEye2.z };

	bool skyHit = Collision::Virtualitys(camera->GetTarget(), skyPos);
	bool UnSkyHit = Collision::UnVirtualitys(camera->GetTarget(), skyPos);

	if (skyHit)
	{
		objCloud->SetPosition(cloudPos);

	}
	if (UnSkyHit)
	{
		//camera->SetEye(oldCameraEye);
		camera->SetTarget(oldCamera);
		objCloud->SetPosition(oldCloudPos);

	}
	objCloud->SetPosition(cloudPos);
	cloudRot.y = atan2f(-fTargetEye.x, -fTargetEye.z);
	cloudRot.y *= 180 / PI;
	objCloud->SetRotation({ 0.0f, cloudRot.y, 0.0f });
}

void GamePlayScene::Draw() {

	DirectXCommon* dxcommon_ = DirectXCommon::GetInstance();

	Object3d::PreDraw(dxcommon_->GetCmdList());
	if (gameFlag == 0) {
		objGround->Draw();
		objSky->Draw();
		objCloud->Draw();

		for (int i = 0; i < enemyNam; i++) {
			objEnemyMov[i]->Draw();
		}
	}
	if (gameFlag == 1) {

	}
	if (gameFlag == 2) {

	}
	Object3d::PostDraw();

	SpriteCommon::GetInstance()->PreDraw();

	if (gameFlag == 0) {
		for (auto& sprite : spritesRader) {
			sprite->Draw();
		}
		for (auto& sprite : spritesEnemy) {
			sprite->Draw();
		}
	}
	if (gameFlag == 1) {
		spriteClear->Draw();
	}
	if (gameFlag == 2) {
		spriteOver->Draw();
	}
}



void GamePlayScene::Create2D_object() {
	sprite = Sprite::Create(0, { 0,0 }, false, false);
	spritesRader.push_back(sprite);
	spriteClear = Sprite::Create(1, { 0,0 }, false, false);
	spriteOver = Sprite::Create(2, { 0,0 }, false, false);

	sprite = Sprite::Create(3, { 0,0 }, false, false);
	sprite->SetPosition({ 1280 - 256,0,0 });
	spritesRader.push_back(sprite);

	sprite = Sprite::Create(4, { 0,0 }, false, false);
	sprite->SetPosition({ 1280 - 256 - 120,0,0 });
	spritesRader.push_back(sprite);

	sprite = Sprite::Create(5, { 0,0 }, false, false);
	sprite->SetPosition({ 1280 - 256,0,0 });
	spritesRader.push_back(sprite);

	for (int i = 0; i < enemyNam; i++) {
		sprite = Sprite::Create(6, { 0,0 }, false, false);
		int ran = rand() % 360 + 1;
		angle[i] = (float)ran;
		sEnemyRe[i] = { 1280 - 256,0 };
		sEnemyRe[i].x += cos((angle[i] * PI) / 180) * 128;
		sEnemyRe[i].y += sin((angle[i] * PI) / 180) * 128;
		sprite->SetPosition({ sEnemyRe[i].x,sEnemyRe[i].y,0 });
		sprite->Update();
		spritesEnemy.push_back(sprite);
		enemyMove[i] = (float)(rand() % 3 + 1);
		enemyMove[i] = enemyMove[i] / 10.0f;
	}
}

void GamePlayScene::ChangeScene() {
	BaseScene* scene = new TitleScene(sceneManager_);
	sceneManager_->SetNextScene(scene);
}

void GamePlayScene::ClassUpdate() {
	camera->Update();
	objGround->Update();
	objSky->Update();
	objCloud->Update();

	for (auto& sprite : spritesRader) {
		sprite->Update();
	}
	for (int i = 0; i < enemyNam; i++) {
		objEnemyMov[i]->Update();
	}

}

void GamePlayScene::SpriteLoadTex() {
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->LoadTexture(0, L"Resources/hud.png");
	spriteCommon->LoadTexture(1, L"Resources/gameClear.png");
	spriteCommon->LoadTexture(2, L"Resources/gameover.png");
	spriteCommon->LoadTexture(3, L"Resources/reader.png");
	spriteCommon->LoadTexture(4, L"Resources/playerRe.png");
	spriteCommon->LoadTexture(5, L"Resources/coraRe.png");
	spriteCommon->LoadTexture(6, L"Resources/enemyRe.png");
}

void GamePlayScene::CameraCreateSet() {
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);

	Object3d::SetCamera(camera);

	camera->SetTarget({ 0,20,0 });
	camera->SetDistance(100.0f);
	camera->SetEye({ 0, 0, 0 });
}
