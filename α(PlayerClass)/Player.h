#pragma once

#include "DebugCamera.h"
#include "Object3d.h"

class Player
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	static void PlayerMouse(Input* input);
	static DebugCamera* PlayerMove(DebugCamera* camera, Input* input);
	static DebugCamera* PlayerVector(DebugCamera* camera, Object3d* objPlayer, XMFLOAT3 playerPos, XMFLOAT3 playerRot);
};