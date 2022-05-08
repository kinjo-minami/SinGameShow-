#include "Player.h"

const float PI = 3.1415926f;
float angleY = 0;
float scaleY = 1.0f / (float)WinApp::window_height;
float distance = 20.0f;
DirectX::XMMATRIX matRot = DirectX::XMMatrixIdentity();

void Player::PlayerMouse(Input* input)
{
	// マウスの入力を取得
	Input::MouseMove mouseMove = input->GetMouseMove();
	float dy = mouseMove.lX * scaleY;
	angleY = -dy * PI;
}

DebugCamera* Player::PlayerMove(DebugCamera* camera, Input* input)
{
	// ボタンが押されていたらカメラを並行移動させる
	if (input->PushKey(DIK_D))
	{
		XMVECTOR move = { 1.0f, 0, 0, 0 };
		move = XMVector3Transform(move, matRot);
		camera->MoveVector(move);
	}
	if (input->PushKey(DIK_A))
	{
		XMVECTOR move = { -1.0f, 0, 0, 0 };
		move = XMVector3Transform(move, matRot);
		camera->MoveVector(move);
	}
	if (input->PushKey(DIK_W))
	{
		XMVECTOR move = { 0, 0, 1.0f, 0 };
		move = XMVector3Transform(move, matRot);
		camera->MoveVector(move);
	}
	if (input->PushKey(DIK_S))
	{
		XMVECTOR move = { 0, 0, -1.0f, 0 };
		move = XMVector3Transform(move, matRot);
		camera->MoveVector(move);
	}

	return camera;
}

void Player::PlayerVector(DebugCamera* camera, Object3d* objPlayer, XMFLOAT3 playerPos, XMFLOAT3 playerRot)
{
	// 追加回転分の回転行列を生成
	XMMATRIX matRotNew = DirectX::XMMatrixIdentity();
	matRotNew *= DirectX::XMMatrixRotationY(-angleY);

	// 累積の回転行列を合成
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

	objPlayer->SetScale({ 1.0f, 1.0f, 1.0f });
	playerPos = { target2.x + fTargetEye.x, target2.y + fTargetEye.y - 1.5f, target2.z + fTargetEye.z };
	objPlayer->SetPosition(playerPos);

	playerRot.y = atan2f(-fTargetEye.x, -fTargetEye.z);
	playerRot.y *= 180 / PI;
	objPlayer->SetRotation({ 0.0f, playerRot.y, 0.0f });
}
