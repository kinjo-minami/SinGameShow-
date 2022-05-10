#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include"SpriteCommon.h"

class Sprite
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
<<<<<<< HEAD
=======
	using XMVECTOR = DirectX::XMVECTOR;
>>>>>>> kinjo(sin)

public:
	struct VertexPosUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT2 uv;  // uv座標
	};

	struct ConstBufferData {
		XMFLOAT4 color; // 色 (RGBA)
		XMMATRIX mat;   // ３Ｄ変換行列
	};

private:
	//頂点バッファ;
	ComPtr<ID3D12Resource> vertBuff_;
	//頂点バッファビュー;
	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	//定数バッファ;
	ComPtr<ID3D12Resource> constBuff_;
	// Z軸回りの回転角
	float rotation_ = 0.0f;
	// 座標
	XMFLOAT3 position_ = { 0,0,0 };
	// ワールド行列
	XMMATRIX matWorld_;
	// 色(RGBA)
	XMFLOAT4 color_ = { 1, 1, 1, 1 };
	// テクスチャ番号
	UINT texNumber_ = 0;
	// 大きさ
	XMFLOAT2 size_ = { 100, 100 };
	// アンカーポイント
	XMFLOAT2 anchorpoint_ = { 0.5f, 0.5f };
	// 左右反転
	bool isFlipX_ = false;
	// 上下反転
	bool isFlipY_ = false;
	// テクスチャ左上座標
	XMFLOAT2 texLeftTop_ = { 0, 0 };
	// テクスチャ切り出しサイズ
	XMFLOAT2 texSize_ = { 100, 100 };
	// 非表示
	bool isInvisible_ = false;
<<<<<<< HEAD
=======
	// 視点座標
	XMFLOAT3 eye = { 0, 0, 0 };
	// 注視点座標
	XMFLOAT3 target = { 0, 0, 0 };
	// 上方向ベクトル
	XMFLOAT3 up = { 0, 1, 0 };
	XMMATRIX matBillboard = DirectX::XMMatrixIdentity();
	// Y軸回りビルボード行列
	XMMATRIX matBillboardY = DirectX::XMMatrixIdentity();
	// ビュー行列
	XMMATRIX matView = DirectX::XMMatrixIdentity();
	bool viewDirty = false;
>>>>>>> kinjo(sin)

	SpriteCommon* SspriteCommon = nullptr;

public:

	static Sprite* Create(SpriteCommon* spriteCommon, UINT texNumber, XMFLOAT2 anchorpoint = { 0.5f,0.5f }, bool isFlipX = false, bool isFlipY = false);

	void initialize(SpriteCommon* spriteCommon, UINT texNumber, XMFLOAT2 anchorpoint = { 0.5f,0.5f }, bool isFlipX = false, bool isFlipY = false);

	void TransferVertexBuffer();

	void Update();

	void SetPosition(const XMFLOAT3 position) { position_ = position; }

	void SetRotation(float rotation) { rotation_ = rotation; }

	void SetSize(XMFLOAT2 size) { size_ = size; }

<<<<<<< HEAD
	void Draw();
=======
	void SetAnchorpoint(XMFLOAT2 anchorpoint) { anchorpoint_ = anchorpoint; }

	void SetTexLeftTop(XMFLOAT2 texLeftTop) { texLeftTop_ = texLeftTop; }

	void MoveVector(const XMVECTOR& move);

	void UpdateViewMatrix();

	XMFLOAT3 GetPosition() { return position_; }

	
	inline const XMFLOAT3& GetEye() {
		return eye;
	}
	inline const XMFLOAT3& GetTarget() {
		return target;
	}
	inline const XMFLOAT3& GetUp() {
		return up;
	}
	inline void SetEye(XMFLOAT3 eye) {
		this->eye = eye; viewDirty = true;
	}
	inline void SetTarget(XMFLOAT3 target) {
		this->target = target; viewDirty = true;
	}
	inline void SetUp(XMFLOAT3 up) {
		this->up = up; viewDirty = true;
	}
	void Draw();
	
>>>>>>> kinjo(sin)

};