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
	delete spritePlayer;



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
	cloudRot = objCloud->GetRotation();

	//enemy複製
	for (int i = 0; i < 800; i++) {
		enemyMovPos[i] = { 0,4,50 };
		objEnemyMov[i] = Object3d::Create();

	}
	for (int i = 0; i < enemyNam; i++) {
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
		Enemymove();
		EnemyPlayerDistance();

	}
	if (gameFlag == 1) {


	}
	if (gameFlag == 2) {


	}

	/*if (input->TriggerKey(DIK_RETURN)) {
		ChangeScene();
	}*/
}

void GamePlayScene::PlayerMove() {
	Input* input = Input::GetInstance();

	// マウスの入力を取得
	Input::MouseMove mouseMove = input->GetMouseMove();
	float dy = mouseMove.lX * scaleY;
	angleY = -dy * XM_PI;
	XMFLOAT3 oldCamera = camera->GetTarget();
	XMFLOAT3 oldCloudPos = cloudPos;
	XMFLOAT3 oldCameraEye = camera->GetEye();

	if (input->PushKey(DIK_D)) {
		XMVECTOR move = { 1.0f, 0, 0, 0 };
		move = XMVector3Transform(move, matRot);
		camera->MoveVector(move);
	}
	if (input->PushKey(DIK_A)) {
		XMVECTOR move = { -1.0f, 0, 0, 0 };
		move = XMVector3Transform(move, matRot);
		camera->MoveVector(move);
	}
	if (input->PushKey(DIK_W)) {
		XMVECTOR move = { 0, 0, 1.0f, 0 };
		move = XMVector3Transform(move, matRot);
		camera->MoveVector(move);
	}
	if (input->PushKey(DIK_S)) {
		XMVECTOR move = { 0, 0, -1.0f, 0 };
		move = XMVector3Transform(move, matRot);
		camera->MoveVector(move);
	}
	spritePlayer->SetPosition(playerRe);

	XMMATRIX matRotNew = XMMatrixIdentity();
	matRotNew *= XMMatrixRotationY(-angleY);
	// 累積の回転行列を合成
	matRot = matRotNew * matRot;

	// 注視点から視点へのベクトルと、上方向ベクトル
	XMVECTOR vTargetEye = { 0.0f, 0.0f, -20, 1.0f };
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

	// 大きさ計算
	length = sqrtf(pow(target2.x - eye.x, 2) + pow(target2.y - eye.y, 2) + pow(target2.z - eye.z, 2));
	fTargetEye.x = eye.x - target2.x;
	fTargetEye.y = eye.y - target2.y;
	fTargetEye.z = eye.z - target2.z;

	fTargetEye.x /= length;
	fTargetEye.y /= length;
	fTargetEye.z /= length;

	fTargetEye.x *= 17;
	fTargetEye.y *= 17;
	fTargetEye.z *= 17;

	objCloud->SetScale({ 1.0f, 1.0f, 1.0f });

	cloudPos = { target2.x + fTargetEye.x, target2.y + fTargetEye.y - 1.5f, target2.z + fTargetEye.z };

	bool skyHit = Collision::Virtualitys(camera->GetTarget(), skyPos);
	bool UnSkyHit = Collision::UnVirtualitys(camera->GetTarget(), skyPos);

	if (skyHit) {
		objCloud->SetPosition(cloudPos);
	}
	if (UnSkyHit) {
		camera->SetTarget(oldCamera);
		objCloud->SetPosition(oldCloudPos);
	}
	objCloud->SetPosition(cloudPos);
	cloudRot.y = atan2f(-fTargetEye.x, -fTargetEye.z);
	cloudRot.y *= 180 / PI;
	objCloud->SetRotation({ 0.0f, cloudRot.y, 0.0f });
	if (skyHit)
	{
		if (input->PushKey(DIK_D))
		{
			playerRe.y += sin(((cloudRot.y + 90) * PI) / 180) * (1.0f / 3.90625f);
			playerRe.x += cos(((cloudRot.y + 90) * PI) / 180) * (1.0f / 3.90625f);
		}
		if (input->PushKey(DIK_A))
		{
			playerRe.y -= sin(((cloudRot.y + 90) * PI) / 180) * (1.0f / 3.90625f);
			playerRe.x -= cos(((cloudRot.y + 90) * PI) / 180) * (1.0f / 3.90625f);

		}
		if (input->PushKey(DIK_W))
		{
			playerRe.y += sin((cloudRot.y * PI) / 180) * (1.0f / 3.90625f);
			playerRe.x += cos((cloudRot.y * PI) / 180) * (1.0f / 3.90625f);
		}
		if (input->PushKey(DIK_S))
		{
			playerRe.y -= sin((cloudRot.y * PI) / 180) * (1.0f / 3.90625f);
			playerRe.x -= cos((cloudRot.y * PI) / 180) * (1.0f / 3.90625f);
		}
	}
	spritePlayer->SetRotation(cloudRot.y + 90);


}



void GamePlayScene::Draw() {

	DirectXCommon* dxcommon_ = DirectXCommon::GetInstance();

	Object3d::PreDraw(dxcommon_->GetCmdList());
	if (gameFlag == 0) {
		objGround->Draw();
		objSky->Draw();
		objCloud->Draw();
		if (enemyWave >= 0) {
			for (int i = 0; i < enemyNam; i++) {
				if (enemyFlag[i] == 0)objEnemyMov[i]->Draw();
			}
		}
	}
	if (enemyWave >= 1) {

	}

	if (enemyWave >= 2) {

	}

	if (enemyWave >= 3) {

	}

	if (enemyWave >= 4) {

	}

	if (enemyWave >= 5) {

	}

	if (enemyWave >= 6) {

	}

	if (enemyWave >= 7) {

	}

	if (enemyWave >= 8) {

	}

	if (enemyWave >= 9) {

	}

	if (enemyWave >= 10) {

	}
	if (enemyWave >= 11) {
		for (int i = (enemyNam * 11) + 7; i < (enemyNam * 12) + 9; i++)
		{
			
		}
	}

	if (enemyWave >= 12) {
		for (int i = (enemyNam * 12) + 9; i < (enemyNam * 13) + (enemyNam + 2); i++)
		{
			
		}
	}
	if (enemyWave >= 13) {

		for (int i = (enemyNam * 13) + (enemyNam + 2); i < (enemyNam * 14) + (enemyNam + 5); i++)
		{

		}
	}

	if (enemyWave >= 14) {
		for (int i = (enemyNam * 14) + (enemyNam + 5); i < (enemyNam * 15) + (enemyNam + 8); i++)
		{

		}

	}

	if (enemyWave >= 15) {
		for (int i = (enemyNam * 15) + (enemyNam + 8); i < (enemyNam * 16) + ((enemyNam * 2) + 1); i++)
		{

		}
	}

	if (enemyWave >= 16) {
		for (int i = (enemyNam * 16) + ((enemyNam * 2) + 1); i < (enemyNam * 17) + ((enemyNam * 2) + 4); i++)
		{

		}
	}

	if (enemyWave >= 17) {
		for (int i = (enemyNam * 17) + ((enemyNam * 2) + 4); i < (enemyNam * 18) + ((enemyNam * 2) + 8); i++)
		{

		}
	}

	if (enemyWave >= 18) {
		for (int i = (enemyNam * 18) + ((enemyNam * 2) + 8); i < (enemyNam * 18) + ((enemyNam * 3) + 2); i++)
		{

		}
	}

	if (enemyWave >= 19) {
		for (int i = (enemyNam * 19) + ((enemyNam * 3) + 2); i < (enemyNam * 20) + ((enemyNam * 3) + 6); i++)
		{

		}
	}

	if (enemyWave >= 20) {
		for (int i = (enemyNam * 20) + ((enemyNam * 3) + 6); i < (enemyNam * 21) + ((enemyNam * 4) + 0); i++)
		{

		}
	}

	if (enemyWave >= 21) {
		for (int i = (enemyNam * 21) + ((enemyNam * 4) + 0); i < (enemyNam * 22) + ((enemyNam * 4) + 4); i++)
		{

		}
	}

	if (enemyWave >= 22) {
		for (int i = (enemyNam * 22) + ((enemyNam * 4) + 4); i < (enemyNam * 23) + ((enemyNam * 4) + 8); i++)
		{

		}
	}

	if (enemyWave >= 23) {
		for (int i = (enemyNam * 23) + ((enemyNam * 4) + 8); i < (enemyNam * 24) + ((enemyNam * 5) + 3); i++)
		{

		}
	}

	if (enemyWave >= 24) {
		for (int i = (enemyNam * 24) + ((enemyNam * 5) + 3); i < (enemyNam * 25) + ((enemyNam * 5) + 8); i++)
		{

		}
	}

	if (enemyWave >= 25) {
		for (int i = (enemyNam * 25) + ((enemyNam * 5) + 8); i < (enemyNam * 26) + ((enemyNam * 6) + 3); i++)
		{

		}
	}

	if (enemyWave >= 26) {
		for (int i = (enemyNam * 26) + ((enemyNam * 6) + 3); i < (enemyNam * 27) + ((enemyNam * 6) + 8); i++)
		{

		}
	}

	if (enemyWave >= 27) {
		for (int i = (enemyNam * 27) + ((enemyNam * 6) + 8); i < (enemyNam * 28) + ((enemyNam * 7) + 3); i++)
		{

		}
	}

	if (enemyWave >= 28) {
		for (int i = (enemyNam * 28) + ((enemyNam * 7) + 3); i < (enemyNam * 29) + ((enemyNam * 7) + 9); i++)
		{

		}
	}

	if (enemyWave >= 29) {
		for (int i = (enemyNam * 29) + ((enemyNam * 7) + 9); i < (enemyNam * 30) + ((enemyNam * 8) + 5); i++)
		{

		}
	}

	if (enemyWave >= 30) {
		for (int i = (enemyNam * 30) + ((enemyNam * 8) + 5); i < (enemyNam * 31) + ((enemyNam * 9) + 1); i++)
		{

		}
	}

	if (enemyWave >= 31) {
		for (int i = (enemyNam * 31) + ((enemyNam * 9) + 1); i < (enemyNam * 32) + ((enemyNam * 9) + 7); i++)
		{

		}
	}

	if (enemyWave >= 32) {
		for (int i = (enemyNam * 32) + ((enemyNam * 9) + 7); i < (enemyNam * 33) + ((enemyNam * 10) + 3); i++)
		{

		}
	}

	if (enemyWave >= 33) {
		for (int i = (enemyNam * 33) + ((enemyNam * 10) + 3); i < (enemyNam * 34) + ((enemyNam * 10) + 9); i++)
		{

		}
	}

	if (enemyWave >= 34) {
		for (int i = (enemyNam * 34) + ((enemyNam * 10) + 9); i < (enemyNam * 35) + ((enemyNam * 11) + 5); i++)
		{

		}
	}

	if (enemyWave >= 35) {
		for (int i = (enemyNam * 35) + ((enemyNam * 11) + 5); i < (enemyNam * 36) + ((enemyNam * 12) + 1); i++)
		{

		}
	}

	if (enemyWave >= 36) {
		for (int i = (enemyNam * 36) + ((enemyNam * 12) + 1); i < (enemyNam * 37) + ((enemyNam * 12) + 7); i++)
		{

		}
	}

	if (enemyWave >= 37) {
		for (int i = (enemyNam * 37) + ((enemyNam * 12) + 7); i < (enemyNam * 38) + ((enemyNam * 13) + 4); i++)
		{

		}
	}

	if (enemyWave >= 38) {
		for (int i = (enemyNam * 38) + ((enemyNam * 13) + 4); i < (enemyNam * 39) + ((enemyNam * 14) + 1); i++)
		{

		}
	}

	if (enemyWave >= 39) {
		for (int i = (enemyNam * 39) + ((enemyNam * 14) + 1); i < (enemyNam * 40) + ((enemyNam * 14) + 8); i++)
		{

		}
	}

	if (enemyWave >= 40) {
		for (int i = (enemyNam * 40) + ((enemyNam * 14) + 8); i < (enemyNam * 41) + ((enemyNam * 15) + 5); i++)
		{

		}
	}

	if (enemyWave >= 41) {
		for (int i = (enemyNam * 41) + ((enemyNam * 15) + 5); i < (enemyNam * 42) + ((enemyNam * 16) + 2); i++)
		{

		}
	}

	if (enemyWave >= 42) {
		for (int i = (enemyNam * 42) + ((enemyNam * 16) + 2); i < (enemyNam * 43) + ((enemyNam * 17) + 0); i++)
		{

		}
	}

	if (enemyWave >= 43) {
		for (int i = (enemyNam * 43) + ((enemyNam * 17) + 0); i < (enemyNam * 44) + ((enemyNam * 17) + 8); i++)
		{

		}
	}

	if (enemyWave >= 44) {
		for (int i = (enemyNam * 44) + ((enemyNam * 17) + 8); i < (enemyNam * 45) + ((enemyNam * 18) + 6); i++)
		{

		}
	}

	if (enemyWave >= 45) {
		for (int i = (enemyNam * 45) + ((enemyNam * 18) + 6); i < (enemyNam * 46) + ((enemyNam * 19) + 4); i++)
		{

		}
	}

	if (enemyWave >= 46) {
		for (int i = (enemyNam * 46) + ((enemyNam * 19) + 4); i < (enemyNam * 47) + ((enemyNam * 20) + 2); i++)
		{

		}
	}

	if (enemyWave >= 47) {
		for (int i = (enemyNam * 47) + ((enemyNam * 20) + 2); i < (enemyNam * 48) + ((enemyNam * 21) + 0); i++)
		{

		}
	}

	if (enemyWave >= 48) {
		for (int i = (enemyNam * 48) + ((enemyNam * 21) + 0); i < (enemyNam * 49) + ((enemyNam * 21) + 8); i++)
		{

		}
	}

	if (enemyWave >= 49) {
		for (int i = (enemyNam * 49) + ((enemyNam * 21) + 8); i < (enemyNam * 50) + ((enemyNam * 22) + 6); i++)
		{

		}
	}

	if (enemyWave >= 50) {
		for (int i = (enemyNam * 50) + ((enemyNam * 22) + 6); i < (enemyNam * 51) + ((enemyNam * 23) + 5); i++)
		{

		}
	}

	if (enemyWave >= 51) {
		for (int i = (enemyNam * 51) + ((enemyNam * 23) + 5); i < (enemyNam * 52) + ((enemyNam * 24) + 4); i++)
		{

		}
	}

	if (enemyWave >= 52) {
		for (int i = (enemyNam * 52) + ((enemyNam * 24) + 4); i < (enemyNam * 53) + ((enemyNam * 25) + 3); i++)
		{

		}
	}

	if (enemyWave >= 53) {
		for (int i = (enemyNam * 53) + ((enemyNam * 25) + 3); i < (enemyNam * 54) + ((enemyNam * 26)); i++)
		{

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
		spritePlayer->Draw();

		for (int i = 0; i < enemyNam*80; i++) {
			if (enemyWave >= 0) {

				if (enemyFlag[i] == 0) { spriteEnemyRe[i]->Draw(); }
				if (enemyWave == 0&&i == enemyNam) { break; }
			}
			if (enemyWave >= 1) {
				if (enemyFlag[i] == 0) { spriteEnemyRe[i]->Draw(); }

			}

			if (enemyWave >= 2) {
				if (enemyFlag[i] == 0) { spriteEnemyRe[i]->Draw(); }

			}

			if (enemyWave >= 3) {
				if (enemyFlag[i] == 0) { spriteEnemyRe[i]->Draw(); }

			}

			if (enemyWave >= 4) {
				if (enemyFlag[i] == 0) { spriteEnemyRe[i]->Draw(); }

			}

			if (enemyWave >= 5) {
				if (enemyFlag[i] == 0) { spriteEnemyRe[i]->Draw(); }

			}

			if (enemyWave >= 6) {
				if (enemyFlag[i] == 0) { spriteEnemyRe[i]->Draw(); }

			}

			if (enemyWave >= 7) {
				if (enemyFlag[i] == 0) { spriteEnemyRe[i]->Draw(); }

			}

			if (enemyWave >= 8) {
				if (enemyFlag[i] == 0) { spriteEnemyRe[i]->Draw(); }

			}

			if (enemyWave >= 9) {
				if (enemyFlag[i] == 0) { spriteEnemyRe[i]->Draw(); }

			}

			if (enemyWave >= 10) {
				if (enemyFlag[i] == 0) { spriteEnemyRe[i]->Draw(); }

			}
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

	spritePlayer = Sprite::Create(4, { 0.5,0.5 }, false, false);
	spritePlayer->SetPosition({ 1280 - 256 - 120,0,0 });

	sprite = Sprite::Create(5, { 0,0 }, false, false);
	sprite->SetPosition({ 1280 - 256,0,0 });
	spritesRader.push_back(sprite);
	for (int i = 0; i < enemyNam * 80; i++)
	{
		spriteEnemyRe[i] = Sprite::Create(6, { 0,0 }, false, false);
		int ran = rand() % 360 + 1;
		angle[i] = (float)ran;
	}
	for (int i = 0; i < enemyNam * 80; i++) {
		sEnemyRe[i] = { 1280 - 256,0 };
		sEnemyRe[i].x += cos((angle[i] * PI) / 180) * 128;
		sEnemyRe[i].y += sin((angle[i] * PI) / 180) * 128;
		spriteEnemyRe[i]->SetPosition({ sEnemyRe[i].x,sEnemyRe[i].y,0 });
		spriteEnemyRe[i]->Update();

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
	if (enemyWave >= 0) {
		
		for (int i = 0; i < enemyNam; i++) {
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }
		}
	}
	if (enemyWave >= 1) {
		for (int i = enemyNam; i < enemyNam * 2; i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 2) {
		for (int i = enemyNam * 2; i < enemyNam * 3; i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 3) {
		for (int i = enemyNam * 3; i < enemyNam * 4; i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }


		}
	}

	if (enemyWave >= 4) {
		for (int i = enemyNam * 4; i < enemyNam * 5; i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 5) {
		for (int i = enemyNam * 5; i < enemyNam * 6; i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 6) {
		for (int i = enemyNam * 6; i < (enemyNam * 7) + 1; i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 7) {
		for (int i = (enemyNam * 7) + 1; i < (enemyNam * 8) + 2; i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 8) {
		for (int i = (enemyNam * 8) + 2; i < (enemyNam * 9) + 3; i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 9) {
		for (int i = (enemyNam * 9) + 3; i < (enemyNam * 10) + 5; i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 10) {
		for (int i = (enemyNam * 10) + 5; i < (enemyNam * 11) + 7; i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}
	if (enemyWave >= 11) {
		for (int i = (enemyNam * 11) + 7; i < (enemyNam * 12) + 9; i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 12) {
		for (int i = (enemyNam * 12) + 9; i < (enemyNam * 13) + (enemyNam + 2); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}
	if (enemyWave >= 13) {

		for (int i = (enemyNam * 13) + (enemyNam + 2); i < (enemyNam * 14) + (enemyNam + 5); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 14) {
		for (int i = (enemyNam * 14) + (enemyNam + 5); i < (enemyNam * 15) + (enemyNam + 8); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}

	}

	if (enemyWave >= 15) {
		for (int i = (enemyNam * 15) + (enemyNam + 8); i < (enemyNam * 16) + ((enemyNam * 2) + 1); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 16) {
		for (int i = (enemyNam * 16) + ((enemyNam * 2) + 1); i < (enemyNam * 17) + ((enemyNam * 2) + 4); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 17) {
		for (int i = (enemyNam * 17) + ((enemyNam * 2) + 4); i < (enemyNam * 18) + ((enemyNam * 2) + 8); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 18) {
		for (int i = (enemyNam * 18) + ((enemyNam * 2) + 8); i < (enemyNam * 18) + ((enemyNam * 3) + 2); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 19) {
		for (int i = (enemyNam * 19) + ((enemyNam * 3) + 2); i < (enemyNam * 20) + ((enemyNam * 3) + 6); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 20) {
		for (int i = (enemyNam * 20) + ((enemyNam * 3) + 6); i < (enemyNam * 21) + ((enemyNam * 4) + 0); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 21) {
		for (int i = (enemyNam * 21) + ((enemyNam * 4) + 0); i < (enemyNam * 22) + ((enemyNam * 4) + 4); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 22) {
		for (int i = (enemyNam * 22) + ((enemyNam * 4) + 4); i < (enemyNam * 23) + ((enemyNam * 4) + 8); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 23) {
		for (int i = (enemyNam * 23) + ((enemyNam * 4) + 8); i < (enemyNam * 24) + ((enemyNam * 5) + 3); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 24) {
		for (int i = (enemyNam * 24) + ((enemyNam * 5) + 3); i < (enemyNam * 25) + ((enemyNam * 5) + 8); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 25) {
		for (int i = (enemyNam * 25) + ((enemyNam * 5) + 8); i < (enemyNam * 26) + ((enemyNam * 6) + 3); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 26) {
		for (int i = (enemyNam * 26) + ((enemyNam * 6) + 3); i < (enemyNam * 27) + ((enemyNam * 6) + 8); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 27) {
		for (int i = (enemyNam * 27) + ((enemyNam * 6) + 8); i < (enemyNam * 28) + ((enemyNam * 7) + 3); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 28) {
		for (int i = (enemyNam * 28) + ((enemyNam * 7) + 3); i < (enemyNam * 29) + ((enemyNam * 7) + 9); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 29) {
		for (int i = (enemyNam * 29) + ((enemyNam * 7) + 9); i < (enemyNam * 30) + ((enemyNam * 8) + 5); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 30) {
		for (int i = (enemyNam * 30) + ((enemyNam * 8) + 5); i < (enemyNam * 31) + ((enemyNam * 9) + 1); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 31) {
		for (int i = (enemyNam * 31) + ((enemyNam * 9) + 1); i < (enemyNam * 32) + ((enemyNam * 9) + 7); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 32) {
		for (int i = (enemyNam * 32) + ((enemyNam * 9) + 7); i < (enemyNam * 33) + ((enemyNam * 10) + 3); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 33) {
		for (int i = (enemyNam * 33) + ((enemyNam * 10) + 3); i < (enemyNam * 34) + ((enemyNam * 10) + 9); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 34) {
		for (int i = (enemyNam * 34) + ((enemyNam * 10) + 9); i < (enemyNam * 35) + ((enemyNam * 11) + 5); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 35) {
		for (int i = (enemyNam * 35) + ((enemyNam * 11) + 5); i < (enemyNam * 36) + ((enemyNam * 12) + 1); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 36) {
		for (int i = (enemyNam * 36) + ((enemyNam * 12) + 1); i < (enemyNam * 37) + ((enemyNam * 12) + 7); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 37) {
		for (int i = (enemyNam * 37) + ((enemyNam * 12) + 7); i < (enemyNam * 38) + ((enemyNam * 13) + 4); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 38) {
		for (int i = (enemyNam * 38) + ((enemyNam * 13) + 4); i < (enemyNam * 39) + ((enemyNam * 14) + 1); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 39) {
		for (int i = (enemyNam * 39) + ((enemyNam * 14) + 1); i < (enemyNam * 40) + ((enemyNam * 14) + 8); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 40) {
		for (int i = (enemyNam * 40) + ((enemyNam * 14) + 8); i < (enemyNam * 41) + ((enemyNam * 15) + 5); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 41) {
		for (int i = (enemyNam * 41) + ((enemyNam * 15) + 5); i < (enemyNam * 42) + ((enemyNam * 16) + 2); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 42) {
		for (int i = (enemyNam * 42) + ((enemyNam * 16) + 2); i < (enemyNam * 43) + ((enemyNam * 17) + 0); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 43) {
		for (int i = (enemyNam * 43) + ((enemyNam * 17) + 0); i < (enemyNam * 44) + ((enemyNam * 17) + 8); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 44) {
		for (int i = (enemyNam * 44) + ((enemyNam * 17) + 8); i < (enemyNam * 45) + ((enemyNam * 18) + 6); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 45) {
		for (int i = (enemyNam * 45) + ((enemyNam * 18) + 6); i < (enemyNam * 46) + ((enemyNam * 19) + 4); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 46) {
		for (int i = (enemyNam * 46) + ((enemyNam * 19) + 4); i < (enemyNam * 47) + ((enemyNam * 20) + 2); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 47) {
		for (int i = (enemyNam * 47) + ((enemyNam * 20) + 2); i < (enemyNam * 48) + ((enemyNam * 21) + 0); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 48) {
		for (int i = (enemyNam * 48) + ((enemyNam * 21) + 0); i < (enemyNam * 49) + ((enemyNam * 21) + 8); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 49) {
		for (int i = (enemyNam * 49) + ((enemyNam * 21) + 8); i < (enemyNam * 50) + ((enemyNam * 22) + 6); i++)
		{

		}
	}

	if (enemyWave >= 50) {
		for (int i = (enemyNam * 50) + ((enemyNam * 22) + 6); i < (enemyNam * 51) + ((enemyNam * 23) + 5); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 51) {
		for (int i = (enemyNam * 51) + ((enemyNam * 23) + 5); i < (enemyNam * 52) + ((enemyNam * 24) + 4); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 52) {
		for (int i = (enemyNam * 52) + ((enemyNam * 24) + 4); i < (enemyNam * 53) + ((enemyNam * 25) + 3); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}

	if (enemyWave >= 53) {
		for (int i = (enemyNam * 53) + ((enemyNam * 25) + 3); i < (enemyNam * 54) + ((enemyNam * 26)); i++)
		{
			if (enemyFlag[i] == 0) { objEnemyMov[i]->Update(); }

		}
	}
	spritePlayer->Update();

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

void GamePlayScene::EnemyPlayerDistance()
{
	if (enemyWave >= 0)
	{
		for (int i = 0; i < enemyNam; i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));
				if (i == 0)
				{
					earliestEnemyNum = 0;
				}
				else
				{
					if (earliest[earliestEnemyNum] > earliest[i])
					{
						earliestEnemyNum = i;
					}
					else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
					{
						earliestEnemyNum = earliestEnemyNum;
					}
					else
					{
						earliestEnemyNum = i;
					}
				}
			}
		}
	}
	if (enemyWave >= 1) {
		for (int i = enemyNam; i < enemyNam * 2; i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 2) {
		for (int i = enemyNam * 2; i < enemyNam * 3; i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 3) {
		for (int i = enemyNam * 3; i < enemyNam * 4; i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 4) {
		for (int i = enemyNam*4; i < enemyNam * 5; i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 5) {
		for (int i = enemyNam*5; i < enemyNam * 6; i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 6) {
		for (int i = enemyNam*6; i < (enemyNam * 7) + 1; i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 7) {
		for (int i = (enemyNam * 7) + 1; i < (enemyNam * 8) + 2; i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 8) {
		for (int i = (enemyNam * 8) + 2; i < (enemyNam * 9) + 3; i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 9) {
		for (int i = (enemyNam * 9) + 3; i < (enemyNam * 10) + 5; i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 10) {
		for (int i = (enemyNam * 10) + 5; i < (enemyNam * 11) + 7; i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}
	if (enemyWave >= 11) {
		for (int i = (enemyNam * 11) + 7; i < (enemyNam * 12) + 9; i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 12) {
		for (int i = (enemyNam * 12) + 9; i < (enemyNam * 13) + (enemyNam + 2); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
			
		}
	}

	if (enemyWave >= 13) {

		for (int i = (enemyNam * 13) + (enemyNam + 2); i < (enemyNam * 14) + (enemyNam + 5); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 14) {
		for (int i = (enemyNam * 14) + (enemyNam + 5); i < (enemyNam * 15) + (enemyNam + 8); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}

	}

	if (enemyWave >= 15) {
		for (int i = (enemyNam * 15) + (enemyNam + 8); i < (enemyNam * 16) + ((enemyNam * 2) + 1); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 16) {
		for (int i = (enemyNam * 16) + ((enemyNam * 2) + 1); i < (enemyNam * 17) + ((enemyNam * 2) + 4); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 17) {
		for (int i = (enemyNam * 17) + ((enemyNam * 2) + 4); i < (enemyNam * 18) + ((enemyNam * 2) + 8); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 18) {
		for (int i = (enemyNam * 18) + ((enemyNam * 2) + 8); i < (enemyNam * 18) + ((enemyNam * 3) + 2); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 19) {
		for (int i = (enemyNam * 19) + ((enemyNam * 3) + 2); i < (enemyNam * 20) + ((enemyNam * 3) + 6); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 20) {
		for (int i = (enemyNam * 20) + ((enemyNam * 3) + 6); i < (enemyNam * 21) + ((enemyNam * 4) + 0); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 21) {
		for (int i = (enemyNam * 21) + ((enemyNam * 4) + 0); i < (enemyNam * 22) + ((enemyNam * 4) + 4); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 22) {
		for (int i = (enemyNam * 22) + ((enemyNam * 4) + 4); i < (enemyNam * 23) + ((enemyNam * 4) + 8); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 23) {
		for (int i = (enemyNam * 23) + ((enemyNam * 4) + 8); i < (enemyNam * 24) + ((enemyNam * 5) + 3); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 24) {
		for (int i = (enemyNam * 24) + ((enemyNam * 5) + 3); i < (enemyNam * 25) + ((enemyNam * 5) + 8); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 25) {
		for (int i = (enemyNam * 25) + ((enemyNam * 5) + 8); i < (enemyNam * 26) + ((enemyNam * 6) + 3); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 26) {
		for (int i = (enemyNam * 26) + ((enemyNam * 6) + 3); i < (enemyNam * 27) + ((enemyNam * 6) + 8); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 27) {
		for (int i = (enemyNam * 27) + ((enemyNam * 6) + 8); i < (enemyNam * 28) + ((enemyNam * 7) + 3); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 28) {
		for (int i = (enemyNam * 28) + ((enemyNam * 7) + 3); i < (enemyNam * 29) + ((enemyNam * 7) + 9); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 29) {
		for (int i = (enemyNam * 29) + ((enemyNam * 7) + 9); i < (enemyNam * 30) + ((enemyNam * 8) + 5); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 30) {
		for (int i = (enemyNam * 30) + ((enemyNam * 8) + 5); i < (enemyNam * 31) + ((enemyNam * 9) + 1); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 31) {
		for (int i = (enemyNam * 31) + ((enemyNam * 9) + 1); i < (enemyNam * 32) + ((enemyNam * 9) + 7); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 32) {
		for (int i = (enemyNam * 32) + ((enemyNam * 9) + 7); i < (enemyNam * 33) + ((enemyNam * 10) + 3); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 33) {
		for (int i = (enemyNam * 33) + ((enemyNam * 10) + 3); i < (enemyNam * 34) + ((enemyNam * 10) + 9); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 34) {
		for (int i = (enemyNam * 34) + ((enemyNam * 10) + 9); i < (enemyNam * 35) + ((enemyNam * 11) + 5); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 35) {
		for (int i = (enemyNam * 35) + ((enemyNam * 11) + 5); i < (enemyNam * 36) + ((enemyNam * 12) + 1); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 36) {
		for (int i = (enemyNam * 36) + ((enemyNam * 12) + 1); i < (enemyNam * 37) + ((enemyNam * 12) + 7); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 37) {
		for (int i = (enemyNam * 37) + ((enemyNam * 12) + 7); i < (enemyNam * 38) + ((enemyNam * 13) + 4); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 38) {
		for (int i = (enemyNam * 38) + ((enemyNam * 13) + 4); i < (enemyNam * 39) + ((enemyNam * 14) + 1); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 39) {
		for (int i = (enemyNam * 39) + ((enemyNam * 14) + 1); i < (enemyNam * 40) + ((enemyNam * 14) + 8); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 40) {
		for (int i = (enemyNam * 40) + ((enemyNam * 14) + 8); i < (enemyNam * 41) + ((enemyNam * 15) + 5); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 41) {
		for (int i = (enemyNam * 41) + ((enemyNam * 15) + 5); i < (enemyNam * 42) + ((enemyNam * 16) + 2); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 42) {
		for (int i = (enemyNam * 42) + ((enemyNam * 16) + 2); i < (enemyNam * 43) + ((enemyNam * 17) + 0); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 43) {
		for (int i = (enemyNam * 43) + ((enemyNam * 17) + 0); i < (enemyNam * 44) + ((enemyNam * 17) + 8); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 44) {
		for (int i = (enemyNam * 44) + ((enemyNam * 17) + 8); i < (enemyNam * 45) + ((enemyNam * 18) + 6); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 45) {
		for (int i = (enemyNam * 45) + ((enemyNam * 18) + 6); i < (enemyNam * 46) + ((enemyNam * 19) + 4); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 46) {
		for (int i = (enemyNam * 46) + ((enemyNam * 19) + 4); i < (enemyNam * 47) + ((enemyNam * 20) + 2); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 47) {
		for (int i = (enemyNam * 47) + ((enemyNam * 20) + 2); i < (enemyNam * 48) + ((enemyNam * 21) + 0); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 48) {
		for (int i = (enemyNam * 48) + ((enemyNam * 21) + 0); i < (enemyNam * 49) + ((enemyNam * 21) + 8); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 49) {
		for (int i = (enemyNam * 49) + ((enemyNam * 21) + 8); i < (enemyNam * 50) + ((enemyNam * 22) + 6); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 50) {
		for (int i = (enemyNam * 50) + ((enemyNam * 22) + 6); i < (enemyNam * 51) + ((enemyNam * 23) + 5); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 51) {
		for (int i = (enemyNam * 51) + ((enemyNam * 23) + 5); i < (enemyNam * 52) + ((enemyNam * 24) + 4); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 52) {
		for (int i = (enemyNam * 52) + ((enemyNam * 24) + 4); i < (enemyNam * 53) + ((enemyNam * 25) + 3); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

	if (enemyWave >= 53) {
		for (int i = (enemyNam * 53) + ((enemyNam * 25) + 3); i < (enemyNam * 54) + ((enemyNam * 26)); i++)
		{
			if (enemyFlag[i] == 0)
			{
				Earliest.x = cloudPos.x - enemyMovPos[i].x;
				Earliest.y = cloudPos.y - enemyMovPos[i].y;
				Earliest.z = cloudPos.z - enemyMovPos[i].z;
				earliest[i] = sqrtf((Earliest.x * Earliest.x) + (Earliest.y * Earliest.y) + (Earliest.z * Earliest.z));

				if (earliest[earliestEnemyNum] > earliest[i])
				{
					earliestEnemyNum = i;
				}
				else if (earliest[earliestEnemyNum] < earliest[i] && enemyFlag[earliestEnemyNum] == 0)
				{
					earliestEnemyNum = earliestEnemyNum;
				}
				else
				{
					earliestEnemyNum = i;
				}

			}
		}
	}

}
void GamePlayScene::Enemymove()
{
	bool isHit = Collision::territory(cloudPos, enemyMovPos[0]);
	if (enemyWave >= 0) {
		//エネミー移動
		for (int i = 0; i < enemyNam; i++)
		{
			if (enemyFlag[i] == 0)
			{
				XMFLOAT3 vel = {};
				vel.x = sin((angle[i] * PI) / 180) * enemyMove[i];
				vel.y = 0.0f;
				vel.z = cos((angle[i] * PI) / 180) * enemyMove[i];
				enemyMovPos[i].x -= vel.x;
				enemyMovPos[i].y -= vel.y;
				enemyMovPos[i].z -= vel.z;
				sEnemyRe[i].x -= cos((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				sEnemyRe[i].y -= sin((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				//spritesEnemy.get_allocator();
				//spriteEnemyRe->GetPosition();
				spriteEnemyRe[i]->SetPosition({ sEnemyRe[i].x,sEnemyRe[i].y,0 });
				spriteEnemyRe[i]->Update();

				objEnemyMov[i]->SetPosition(enemyMovPos[i]);
				objEnemyMov[i]->Update();
			}
			if (enemyFlag[i] == 1) {
				delete spriteEnemyRe[i];
				delete objEnemyMov[i];
				enemyCount++;
				if (enemyCount >= 7&& enemyWaveFlag == 0)
				{
					enemyWaveFlag++;
					enemyWave++;
				}
				score += 1000;
			}

		}

	}

	

	if (enemyWave >= 1) {
		for (int i = enemyNam; i < enemyNam * 2; i++)
		{
			if (enemyFlag[i] == 0)
			{
				XMFLOAT3 vel = {};
				vel.x = sin((angle[i] * PI) / 180) * enemyMove[i];
				vel.y = 0.0f;
				vel.z = cos((angle[i] * PI) / 180) * enemyMove[i];
				enemyMovPos[i].x -= vel.x;
				enemyMovPos[i].y -= vel.y;
				enemyMovPos[i].z -= vel.z;
				sEnemyRe[i].x -= cos((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				sEnemyRe[i].y -= sin((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				//spritesEnemy.get_allocator();
				//spriteEnemyRe->GetPosition();
				spriteEnemyRe[i]->SetPosition({ sEnemyRe[i].x,sEnemyRe[i].y,0 });
				spriteEnemyRe[i]->Update();

				objEnemyMov[i]->SetPosition(enemyMovPos[i]);
				objEnemyMov[i]->Update();
			}
			if (enemyFlag[i] == 1) {
				delete spriteEnemyRe[i];
				delete objEnemyMov[i];
				enemyCount++;
				if (enemyCount >= 16 && enemyWaveFlag == 1)
				{
					enemyWaveFlag++;
					enemyWave++;
				}
				score += 1000;
			}

		}
	}

	if (enemyWave >= 2) {
		for (int i = enemyNam * 2; i < enemyNam * 3; i++)
		{
			if (enemyFlag[i] == 0)
			{
				XMFLOAT3 vel = {};
				vel.x = sin((angle[i] * PI) / 180) * enemyMove[i];
				vel.y = 0.0f;
				vel.z = cos((angle[i] * PI) / 180) * enemyMove[i];
				enemyMovPos[i].x -= vel.x;
				enemyMovPos[i].y -= vel.y;
				enemyMovPos[i].z -= vel.z;
				sEnemyRe[i].x -= cos((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				sEnemyRe[i].y -= sin((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				//spritesEnemy.get_allocator();
				//spriteEnemyRe->GetPosition();
				spriteEnemyRe[i]->SetPosition({ sEnemyRe[i].x,sEnemyRe[i].y,0 });
				spriteEnemyRe[i]->Update();

				objEnemyMov[i]->SetPosition(enemyMovPos[i]);
				objEnemyMov[i]->Update();
			}
			if (enemyFlag[i] == 1) {
				delete spriteEnemyRe[i];
				delete objEnemyMov[i];
				enemyCount++;
				if (enemyCount >= 26 && enemyWaveFlag == 2)
				{
					enemyWaveFlag++;
					enemyWave++;
				}
				score += 1000;
			}

		}
	}

	if (enemyWave >= 3) {
		for (int i = enemyNam * 3; i < enemyNam * 4; i++)
		{
			if (enemyFlag[i] == 0)
			{
				XMFLOAT3 vel = {};
				vel.x = sin((angle[i] * PI) / 180) * enemyMove[i];
				vel.y = 0.0f;
				vel.z = cos((angle[i] * PI) / 180) * enemyMove[i];
				enemyMovPos[i].x -= vel.x;
				enemyMovPos[i].y -= vel.y;
				enemyMovPos[i].z -= vel.z;
				sEnemyRe[i].x -= cos((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				sEnemyRe[i].y -= sin((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				//spritesEnemy.get_allocator();
				//spriteEnemyRe->GetPosition();
				spriteEnemyRe[i]->SetPosition({ sEnemyRe[i].x,sEnemyRe[i].y,0 });
				spriteEnemyRe[i]->Update();

				objEnemyMov[i]->SetPosition(enemyMovPos[i]);
				objEnemyMov[i]->Update();
			}
			if (enemyFlag[i] == 1) {
				delete spriteEnemyRe[i];
				delete objEnemyMov[i];
				enemyCount++;
				if (enemyCount >= 36 && enemyWaveFlag == 3)
				{
					enemyWaveFlag++;
					enemyWave++;
				}
				score += 1000;
			}

		}
	}

	if (enemyWave >= 4) {
		for (int i = enemyNam * 4; i < enemyNam * 5; i++)
		{
			if (enemyFlag[i] == 0)
			{
				XMFLOAT3 vel = {};
				vel.x = sin((angle[i] * PI) / 180) * enemyMove[i];
				vel.y = 0.0f;
				vel.z = cos((angle[i] * PI) / 180) * enemyMove[i];
				enemyMovPos[i].x -= vel.x;
				enemyMovPos[i].y -= vel.y;
				enemyMovPos[i].z -= vel.z;
				sEnemyRe[i].x -= cos((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				sEnemyRe[i].y -= sin((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				//spritesEnemy.get_allocator();
				//spriteEnemyRe->GetPosition();
				spriteEnemyRe[i]->SetPosition({ sEnemyRe[i].x,sEnemyRe[i].y,0 });
				spriteEnemyRe[i]->Update();

				objEnemyMov[i]->SetPosition(enemyMovPos[i]);
				objEnemyMov[i]->Update();
			}
			if (enemyFlag[i] == 1) {
				delete spriteEnemyRe[i];
				delete objEnemyMov[i];
				enemyCount++;
				if (enemyCount >= 46 && enemyWaveFlag == 4)
				{
					enemyWaveFlag++;
					enemyWave++;
				}
				score += 1000;
			}

		}
	}

	if (enemyWave >= 5) {
		for (int i = enemyNam * 5; i < enemyNam * 6; i++)
		{
			if (enemyFlag[i] == 0)
			{
				XMFLOAT3 vel = {};
				vel.x = sin((angle[i] * PI) / 180) * enemyMove[i];
				vel.y = 0.0f;
				vel.z = cos((angle[i] * PI) / 180) * enemyMove[i];
				enemyMovPos[i].x -= vel.x;
				enemyMovPos[i].y -= vel.y;
				enemyMovPos[i].z -= vel.z;
				sEnemyRe[i].x -= cos((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				sEnemyRe[i].y -= sin((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				//spritesEnemy.get_allocator();
				//spriteEnemyRe->GetPosition();
				spriteEnemyRe[i]->SetPosition({ sEnemyRe[i].x,sEnemyRe[i].y,0 });
				spriteEnemyRe[i]->Update();

				objEnemyMov[i]->SetPosition(enemyMovPos[i]);
				objEnemyMov[i]->Update();
			}
			if (enemyFlag[i] == 1) {
				delete spriteEnemyRe[i];
				delete objEnemyMov[i];
				enemyCount++;
				if (enemyCount >= 56 && enemyWaveFlag == 5)
				{
					enemyWaveFlag++;
					enemyWave++;
				}
				score += 1000;
			}

		}
	}

	if (enemyWave >= 6) {
		for (int i = enemyNam * 6; i < (enemyNam *7)+1; i++)
		{
			if (enemyFlag[i] == 0)
			{
				XMFLOAT3 vel = {};
				vel.x = sin((angle[i] * PI) / 180) * enemyMove[i];
				vel.y = 0.0f;
				vel.z = cos((angle[i] * PI) / 180) * enemyMove[i];
				enemyMovPos[i].x -= vel.x;
				enemyMovPos[i].y -= vel.y;
				enemyMovPos[i].z -= vel.z;
				sEnemyRe[i].x -= cos((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				sEnemyRe[i].y -= sin((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				//spritesEnemy.get_allocator();
				//spriteEnemyRe->GetPosition();
				spriteEnemyRe[i]->SetPosition({ sEnemyRe[i].x,sEnemyRe[i].y,0 });
				spriteEnemyRe[i]->Update();

				objEnemyMov[i]->SetPosition(enemyMovPos[i]);
				objEnemyMov[i]->Update();
			}
			if (enemyFlag[i] == 1) {
				delete spriteEnemyRe[i];
				delete objEnemyMov[i];
				enemyCount++;
				if (enemyCount >= 66 && enemyWaveFlag == 6)
				{
					enemyWaveFlag++;
					enemyWave++;
				}
				score += 1000;
			}

		}
	}

	if (enemyWave >= 7) {
		for (int i = (enemyNam * 7) + 1; i < (enemyNam * 8) + 2; i++)
		{
			if (enemyFlag[i] == 0)
			{
				XMFLOAT3 vel = {};
				vel.x = sin((angle[i] * PI) / 180) * enemyMove[i];
				vel.y = 0.0f;
				vel.z = cos((angle[i] * PI) / 180) * enemyMove[i];
				enemyMovPos[i].x -= vel.x;
				enemyMovPos[i].y -= vel.y;
				enemyMovPos[i].z -= vel.z;
				sEnemyRe[i].x -= cos((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				sEnemyRe[i].y -= sin((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				//spritesEnemy.get_allocator();
				//spriteEnemyRe->GetPosition();
				spriteEnemyRe[i]->SetPosition({ sEnemyRe[i].x,sEnemyRe[i].y,0 });
				spriteEnemyRe[i]->Update();

				objEnemyMov[i]->SetPosition(enemyMovPos[i]);
				objEnemyMov[i]->Update();
			}
			if (enemyFlag[i] == 1) {
				delete spriteEnemyRe[i];
				delete objEnemyMov[i];
				enemyCount++;
				if (enemyCount >= 76 && enemyWaveFlag == 7)
				{
					enemyWaveFlag++;
					enemyWave++;
				}
				score += 1000;
			}

		}
	}

	if (enemyWave >= 8) {
		for (int i = (enemyNam * 8) + 2; i < (enemyNam * 9) + 3; i++)
		{
			if (enemyFlag[i] == 0)
			{
				XMFLOAT3 vel = {};
				vel.x = sin((angle[i] * PI) / 180) * enemyMove[i];
				vel.y = 0.0f;
				vel.z = cos((angle[i] * PI) / 180) * enemyMove[i];
				enemyMovPos[i].x -= vel.x;
				enemyMovPos[i].y -= vel.y;
				enemyMovPos[i].z -= vel.z;
				sEnemyRe[i].x -= cos((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				sEnemyRe[i].y -= sin((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				//spritesEnemy.get_allocator();
				//spriteEnemyRe->GetPosition();
				spriteEnemyRe[i]->SetPosition({ sEnemyRe[i].x,sEnemyRe[i].y,0 });
				spriteEnemyRe[i]->Update();

				objEnemyMov[i]->SetPosition(enemyMovPos[i]);
				objEnemyMov[i]->Update();
			}
			if (enemyFlag[i] == 1) {
				delete spriteEnemyRe[i];
				delete objEnemyMov[i];
				enemyCount++;
				if (enemyCount >= 86 && enemyWaveFlag == 8)
				{
					enemyWaveFlag++;
					enemyWave++;
				}
				score += 1000;
			}

		}
	}

	if (enemyWave >= 9) {
		for (int i = (enemyNam * 9) + 3; i < (enemyNam * 10) + 5; i++)
		{
			if (enemyFlag[i] == 0)
			{
				XMFLOAT3 vel = {};
				vel.x = sin((angle[i] * PI) / 180) * enemyMove[i];
				vel.y = 0.0f;
				vel.z = cos((angle[i] * PI) / 180) * enemyMove[i];
				enemyMovPos[i].x -= vel.x;
				enemyMovPos[i].y -= vel.y;
				enemyMovPos[i].z -= vel.z;
				sEnemyRe[i].x -= cos((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				sEnemyRe[i].y -= sin((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				//spritesEnemy.get_allocator();
				//spriteEnemyRe->GetPosition();
				spriteEnemyRe[i]->SetPosition({ sEnemyRe[i].x,sEnemyRe[i].y,0 });
				spriteEnemyRe[i]->Update();

				objEnemyMov[i]->SetPosition(enemyMovPos[i]);
				objEnemyMov[i]->Update();
			}
			if (enemyFlag[i] == 1) {
				delete spriteEnemyRe[i];
				delete objEnemyMov[i];
				enemyCount++;
				if (enemyCount >= 96 && enemyWaveFlag == 9)
				{
					enemyWaveFlag++;
					enemyWave++;
				}
				score += 1000;
			}

		}
	}

	if (enemyWave >= 10) {
		for (int i = (enemyNam * 10) + 5; i < (enemyNam * 11) + 7; i++)
		{
			if (enemyFlag[i] == 0)
			{
				XMFLOAT3 vel = {};
				vel.x = sin((angle[i] * PI) / 180) * enemyMove[i];
				vel.y = 0.0f;
				vel.z = cos((angle[i] * PI) / 180) * enemyMove[i];
				enemyMovPos[i].x -= vel.x;
				enemyMovPos[i].y -= vel.y;
				enemyMovPos[i].z -= vel.z;
				sEnemyRe[i].x -= cos((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				sEnemyRe[i].y -= sin((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				//spritesEnemy.get_allocator();
				//spriteEnemyRe->GetPosition();
				spriteEnemyRe[i]->SetPosition({ sEnemyRe[i].x,sEnemyRe[i].y,0 });
				spriteEnemyRe[i]->Update();

				objEnemyMov[i]->SetPosition(enemyMovPos[i]);
				objEnemyMov[i]->Update();
			}
			if (enemyFlag[i] == 1) {
				delete spriteEnemyRe[i];
				delete objEnemyMov[i];
				enemyCount++;
				if (enemyCount >= 110 && enemyWaveFlag == 10)
				{
					enemyWaveFlag++;
					enemyWave++;
				}
				score += 1000;
			}

		}
	}

	if (enemyWave >= 11) {
		for (int i = (enemyNam * 11) + 7; i < (enemyNam * 12) + 9; i++)
		{
			if (enemyFlag[i] == 0)
			{
				XMFLOAT3 vel = {};
				vel.x = sin((angle[i] * PI) / 180) * enemyMove[i];
				vel.y = 0.0f;
				vel.z = cos((angle[i] * PI) / 180) * enemyMove[i];
				enemyMovPos[i].x -= vel.x;
				enemyMovPos[i].y -= vel.y;
				enemyMovPos[i].z -= vel.z;
				sEnemyRe[i].x -= cos((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				sEnemyRe[i].y -= sin((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				//spritesEnemy.get_allocator();
				//spriteEnemyRe->GetPosition();
				spriteEnemyRe[i]->SetPosition({ sEnemyRe[i].x,sEnemyRe[i].y,0 });
				spriteEnemyRe[i]->Update();

				objEnemyMov[i]->SetPosition(enemyMovPos[i]);
				objEnemyMov[i]->Update();
			}
			if (enemyFlag[i] == 1) {
				delete spriteEnemyRe[i];
				delete objEnemyMov[i];
				enemyCount++;
				if (enemyCount >= 120 && enemyWaveFlag == 11)
				{
					enemyWaveFlag++;
					enemyWave++;
				}
				score += 1000;
			}

		}
	}

	if (enemyWave >= 12) {
		for (int i = (enemyNam * 12) + 9; i < (enemyNam * 13) + (enemyNam + 2); i++)
		{
			if (enemyFlag[i] == 0)
			{
				XMFLOAT3 vel = {};
				vel.x = sin((angle[i] * PI) / 180) * enemyMove[i];
				vel.y = 0.0f;
				vel.z = cos((angle[i] * PI) / 180) * enemyMove[i];
				enemyMovPos[i].x -= vel.x;
				enemyMovPos[i].y -= vel.y;
				enemyMovPos[i].z -= vel.z;
				sEnemyRe[i].x -= cos((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				sEnemyRe[i].y -= sin((angle[i] * PI) / 180) * (enemyMove[i] / 3.90625f);
				//spritesEnemy.get_allocator();
				//spriteEnemyRe->GetPosition();
				spriteEnemyRe[i]->SetPosition({ sEnemyRe[i].x,sEnemyRe[i].y,0 });
				spriteEnemyRe[i]->Update();

				objEnemyMov[i]->SetPosition(enemyMovPos[i]);
				objEnemyMov[i]->Update();
			}
			if (enemyFlag[i] == 1) {
				delete spriteEnemyRe[i];
				delete objEnemyMov[i];
				enemyCount++;
				if (enemyCount >= 139 && enemyWaveFlag == 11)
				{
					enemyWaveFlag++;
					enemyWave++;
				}
				score += 1000;
			}

		}
	}

	if (enemyWave >= 13) {
		
		for (int i = (enemyNam * 13) + (enemyNam + 2); i < (enemyNam * 14) + (enemyNam + 5); i++)
		{

		}
	}

	if (enemyWave >= 14) {
		for (int i = (enemyNam * 14) + (enemyNam + 5); i < (enemyNam * 15) + (enemyNam + 8); i++)
		{

		}

	}

	if (enemyWave >= 15) {
		for (int i = (enemyNam * 15) + (enemyNam + 8); i < (enemyNam * 16) + ((enemyNam*2) + 1); i++)
		{

		}
	}

	if (enemyWave >= 16) {
		for (int i = (enemyNam * 16) + ((enemyNam * 2) + 1); i < (enemyNam * 17) + ((enemyNam * 2) + 4); i++)
		{

		}
	}

	if (enemyWave >= 17) {
		for (int i = (enemyNam * 17) + ((enemyNam * 2) + 4); i < (enemyNam * 18) + ((enemyNam * 2) + 8); i++)
		{

		}
	}

	if (enemyWave >= 18) {
		for (int i = (enemyNam * 18) + ((enemyNam * 2) + 8); i < (enemyNam * 18) + ((enemyNam * 3) + 2); i++)
		{

		}
	}

	if (enemyWave >= 19) {
		for (int i = (enemyNam * 19) + ((enemyNam * 3) + 2); i < (enemyNam * 20) + ((enemyNam * 3) + 6); i++)
		{

		}
	}

	if (enemyWave >= 20) {
		for (int i = (enemyNam * 20) + ((enemyNam * 3) + 6); i < (enemyNam * 21) + ((enemyNam * 4) + 0); i++)
		{

		}
	}

	if (enemyWave >= 21) {
		for (int i = (enemyNam * 21) + ((enemyNam * 4) + 0); i < (enemyNam * 22) + ((enemyNam * 4) + 4); i++)
		{

		}
	}

	if (enemyWave >= 22) {
		for (int i = (enemyNam * 22) + ((enemyNam * 4) + 4); i < (enemyNam * 23) + ((enemyNam * 4) + 8); i++)
		{

		}
	}

	if (enemyWave >= 23) {
		for (int i = (enemyNam * 23) + ((enemyNam * 4) + 8); i < (enemyNam * 24) + ((enemyNam * 5) + 3); i++)
		{

		}
	}

	if (enemyWave >= 24) {
		for (int i = (enemyNam * 24) + ((enemyNam * 5) + 3); i < (enemyNam * 25) + ((enemyNam * 5) + 8); i++)
		{

		}
	}

	if (enemyWave >= 25) {
		for (int i = (enemyNam * 25) + ((enemyNam * 5) + 8); i < (enemyNam * 26) + ((enemyNam * 6) + 3); i++)
		{

		}
	}

	if (enemyWave >= 26) {
		for (int i = (enemyNam * 26) + ((enemyNam * 6) + 3); i < (enemyNam * 27) + ((enemyNam * 6) + 8); i++)
		{

		}
	}

	if (enemyWave >= 27) {
		for (int i = (enemyNam * 27) + ((enemyNam * 6) + 8); i < (enemyNam * 28) + ((enemyNam * 7) + 3); i++)
		{

		}
	}

	if (enemyWave >= 28) {
		for (int i = (enemyNam * 28) + ((enemyNam * 7) + 3); i < (enemyNam * 29) + ((enemyNam * 7) + 9); i++)
		{

		}
	}

	if (enemyWave >= 29) {
		for (int i = (enemyNam * 29) + ((enemyNam * 7) + 9); i < (enemyNam * 30) + ((enemyNam * 8) + 5); i++)
		{

		}
	}

	if (enemyWave >= 30) {
		for (int i = (enemyNam * 30) + ((enemyNam * 8) + 5); i < (enemyNam * 31) + ((enemyNam * 9) + 1); i++)
		{

		}
	}

	if (enemyWave >= 31) {
		for (int i = (enemyNam * 31) + ((enemyNam * 9) + 1); i < (enemyNam * 32) + ((enemyNam * 9) + 7); i++)
		{

		}
	}

	if (enemyWave >= 32) {
		for (int i = (enemyNam * 32) + ((enemyNam * 9) + 7); i < (enemyNam * 33) + ((enemyNam * 10) + 3); i++)
		{

		}
	}

	if (enemyWave >= 33) {
		for (int i = (enemyNam * 33) + ((enemyNam * 10) + 3); i < (enemyNam * 34) + ((enemyNam * 10) + 9); i++)
		{

		}
	}

	if (enemyWave >= 34) {
		for (int i = (enemyNam * 34) + ((enemyNam * 10) + 9); i < (enemyNam * 35) + ((enemyNam * 11) + 5); i++)
		{

		}
	}

	if (enemyWave >= 35) {
		for (int i = (enemyNam * 35) + ((enemyNam * 11) + 5); i < (enemyNam * 36) + ((enemyNam * 12) + 1); i++)
		{

		}
	}

	if (enemyWave >= 36) {
		for (int i = (enemyNam * 36) + ((enemyNam * 12) + 1); i < (enemyNam * 37) + ((enemyNam * 12) + 7); i++)
		{

		}
	}

	if (enemyWave >= 37) {
		for (int i = (enemyNam * 37) + ((enemyNam * 12) + 7); i < (enemyNam * 38) + ((enemyNam * 13) + 4); i++)
		{

		}
	}

	if (enemyWave >= 38) {
		for (int i = (enemyNam * 38) + ((enemyNam * 13) + 4); i < (enemyNam * 39) + ((enemyNam * 14) + 1); i++)
		{

		}
	}

	if (enemyWave >= 39) {
		for (int i = (enemyNam * 39) + ((enemyNam * 14) + 1); i < (enemyNam * 40) + ((enemyNam * 14) + 8); i++)
		{

		}
	}

	if (enemyWave >= 40) {
		for (int i = (enemyNam * 40) + ((enemyNam * 14) + 8); i < (enemyNam * 41) + ((enemyNam * 15) + 5); i++)
		{

		}
	}

	if (enemyWave >= 41) {
		for (int i = (enemyNam * 41) + ((enemyNam * 15) + 5); i < (enemyNam * 42) + ((enemyNam * 16) + 2); i++)
		{

		}
	}

	if (enemyWave >= 42) {
		for (int i = (enemyNam * 42) + ((enemyNam * 16) + 2); i < (enemyNam * 43) + ((enemyNam * 17) + 0); i++)
		{

		}
	}

	if (enemyWave >= 43) {
		for (int i = (enemyNam * 43) + ((enemyNam * 17) + 0); i < (enemyNam * 44) + ((enemyNam * 17) + 8); i++)
		{

		}
	}

	if (enemyWave >= 44) {
		for (int i = (enemyNam * 44) + ((enemyNam * 17) + 8); i < (enemyNam * 45) + ((enemyNam * 18) + 6); i++)
		{

		}
	}

	if (enemyWave >= 45) {
		for (int i = (enemyNam * 45) + ((enemyNam * 18) + 6); i < (enemyNam * 46) + ((enemyNam * 19) + 4); i++)
		{

		}
	}

	if (enemyWave >= 46) {
		for (int i = (enemyNam * 46) + ((enemyNam * 19) + 4); i < (enemyNam * 47) + ((enemyNam * 20) + 2); i++)
		{

		}
	}

	if (enemyWave >= 47) {
		for (int i = (enemyNam * 47) + ((enemyNam * 20) + 2); i < (enemyNam * 48) + ((enemyNam * 21) + 0); i++)
		{

		}
	}

	if (enemyWave >= 48) {
		for (int i = (enemyNam * 48) + ((enemyNam * 21) + 0); i < (enemyNam * 49) + ((enemyNam * 21) + 8); i++)
		{

		}
	}

	if (enemyWave >= 49) {
		for (int i = (enemyNam * 49) + ((enemyNam * 21) + 8); i < (enemyNam * 50) + ((enemyNam * 22) + 6); i++)
		{

		}
	}

	if (enemyWave >= 50) {
		for (int i = (enemyNam * 50) + ((enemyNam * 22) + 6); i < (enemyNam * 51) + ((enemyNam * 23) + 5); i++)
		{

		}
	}

	if (enemyWave >= 51) {
		for (int i = (enemyNam * 51) + ((enemyNam * 23) + 5); i < (enemyNam * 52) + ((enemyNam * 24) + 4); i++)
		{

		}
	}

	if (enemyWave >= 52) {
		for (int i = (enemyNam * 52) + ((enemyNam * 24) + 4); i < (enemyNam * 53) + ((enemyNam * 25) + 3); i++)
		{

		}
	}
	
	if (enemyWave >= 53) {
		for (int i = (enemyNam * 53) + ((enemyNam * 25) + 3); i < (enemyNam * 54) + ((enemyNam * 26)); i++)
		{

		}
	}
}
