#include"Sprite.h"
#include<d3dx12.h>
using namespace DirectX;
using namespace Microsoft::WRL;
Sprite* Sprite::Create(SpriteCommon* spriteCommon, UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	Sprite* instance = new Sprite();

	instance->initialize(spriteCommon, texNumber, anchorpoint, isFlipX, isFlipY);

	return instance;
}

void Sprite::initialize(SpriteCommon* spriteCommon, UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	HRESULT result = S_FALSE;


	// テクスチャ番号をコピー
	texNumber_ = texNumber;

	// アンカーポイントをコピー
	anchorpoint_ = anchorpoint;

	// 反転フラグをコピー
	isFlipX_ = isFlipX;
	isFlipY_ = isFlipY;

	SspriteCommon = spriteCommon;

	// 頂点データ
	VertexPosUv vertices[4];

	// 指定番号の画像が読み込み済みなら
	if (SspriteCommon->GetTexBuff(texNumber_)) {
		// テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = SspriteCommon->GetTexBuff(texNumber_)->GetDesc();

		// スプライトの大きさを画像の解像度に合わせる
		size_ = { (float)resDesc.Width, (float)resDesc.Height };
		texSize_ = { (float)resDesc.Width, (float)resDesc.Height };
	}

	// 頂点バッファ生成
	result = SspriteCommon->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff_));

	// 頂点バッファデータ転送
	TransferVertexBuffer();

	// 頂点バッファビューの作成
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeof(vertices);
	vbView_.StrideInBytes = sizeof(vertices[0]);

	// 定数バッファの生成
	result = SspriteCommon->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff_));

	// 定数バッファにデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1); // 色指定（RGBA）
	constMap->mat = spriteCommon->GetMatProjection();
	constBuff_->Unmap(0, nullptr);

}

void Sprite::TransferVertexBuffer()
{
	HRESULT result = S_FALSE;

	// 頂点データ
	VertexPosUv vertices[] = {
		//     u     v
		{{}, {0.0f, 1.0f}}, // 左下
		{{}, {0.0f, 0.0f}}, // 左上
		{{}, {1.0f, 1.0f}}, // 右下
		{{}, {1.0f, 0.0f}}, // 右上
	};

	// 左下、左上、右下、右上
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorpoint_.x) * size_.x;
	float right = (1.0f - anchorpoint_.x) * size_.x;
	float top = (0.0f - anchorpoint_.y) * size_.y;
	float bottom = (1.0f - anchorpoint_.y) * size_.y;

	if (isFlipX_)
	{// 左右入れ替え
		left = -left;
		right = -right;
	}

	if (isFlipY_)
	{// 左右入れ替え
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left, bottom,  0.0f }; // 左下
	vertices[LT].pos = { left, top,     0.0f }; // 左上
	vertices[RB].pos = { right, bottom, 0.0f }; // 右下
	vertices[RT].pos = { right, top,    0.0f }; // 右上

	// 指定番号の画像が読み込み済みなら
	if (SspriteCommon->GetTexBuff(texNumber_)) {
		// テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = SspriteCommon->GetTexBuff(texNumber_)->GetDesc();

		float tex_left = texLeftTop_.x / resDesc.Width;
		float tex_right = (texLeftTop_.x + texSize_.x) / resDesc.Width;
		float tex_top = texLeftTop_.y / resDesc.Height;
		float tex_bottom = (texLeftTop_.y + texSize_.y) / resDesc.Height;

		vertices[LB].uv = { tex_left,   tex_bottom }; // 左下
		vertices[LT].uv = { tex_left,   tex_top }; // 左上
		vertices[RB].uv = { tex_right,  tex_bottom }; // 右下
		vertices[RT].uv = { tex_right,  tex_top }; // 右上
	}

	// 頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	vertBuff_->Unmap(0, nullptr);
}

void Sprite::Update()
{
	// ワールド行列の更新
	matWorld_ = XMMatrixIdentity();
	// Z軸回転
	matWorld_ *= XMMatrixRotationZ(XMConvertToRadians(rotation_));
	// 平行移動
	matWorld_ *= XMMatrixTranslation(position_.x, position_.y, position_.z);

	// 定数バッファの転送
	ConstBufferData* constMap = nullptr;
	HRESULT result = constBuff_->Map(0, nullptr, (void**)&constMap);
	constMap->mat = matWorld_ * SspriteCommon->GetMatProjection();
	constMap->color = color_;
	constBuff_->Unmap(0, nullptr);
	//SetTexLeftTop(texLeftTop_);
	TransferVertexBuffer();
}

void Sprite::MoveVector(const XMVECTOR& move)
{
	/*XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;*/
	// 視点と注視点座標を移動し、反映
	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x += move.m128_f32[0];
	eye_moved.y += move.m128_f32[1];
	eye_moved.z += move.m128_f32[2];

	target_moved.x += move.m128_f32[0];
	target_moved.y += move.m128_f32[1];
	target_moved.z += move.m128_f32[2];

	SetEye(eye_moved);
	SetTarget(target_moved);
}

void Sprite::Draw()
{
	if (isInvisible_) {
		return;
	}

	ID3D12GraphicsCommandList* cmdList = SspriteCommon->GetCmd();


	// 頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &vbView_);

	// 定数バッファをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());

	// シェーダリソースビューをセット
	SspriteCommon->SetGraphicsRootDescriptorTable(1, texNumber_);
	// ポリゴンの描画（4頂点で四角形）
	cmdList->DrawInstanced(4, 1, 0, 0);
}




#pragma region 木曜確認

//void Sprite::UpdateViewMatrix()
//{
//	// 視点座標
//	XMVECTOR eyePosition = XMLoadFloat3(&eye);
//	// 注視点座標
//	XMVECTOR targetPosition = XMLoadFloat3(&target);
//	// （仮の）上方向
//	XMVECTOR upVector = XMLoadFloat3(&up);
//
//	// カメラZ軸（視線方向）
//	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
//	// 0ベクトルだと向きが定まらないので除外
//	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
//	assert(!XMVector3IsInfinite(cameraAxisZ));
//	assert(!XMVector3Equal(upVector, XMVectorZero()));
//	assert(!XMVector3IsInfinite(upVector));
//	// ベクトルを正規化
//	cameraAxisZ = XMVector3Normalize(cameraAxisZ);
//
//	// カメラのX軸（右方向）
//	XMVECTOR cameraAxisX;
//	// X軸は上方向→Z軸の外積で求まる
//	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
//	// ベクトルを正規化
//	cameraAxisX = XMVector3Normalize(cameraAxisX);
//
//	// カメラのY軸（上方向）
//	XMVECTOR cameraAxisY;
//	// Y軸はZ軸→X軸の外積で求まる
//	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
//
//	// ここまでで直交した3方向のベクトルが揃う
//	//（ワールド座標系でのカメラの右方向、上方向、前方向）	
//
//	// カメラ回転行列
//	XMMATRIX matCameraRot;
//	// カメラ座標系→ワールド座標系の変換行列
//	matCameraRot.r[0] = cameraAxisX;
//	matCameraRot.r[1] = cameraAxisY;
//	matCameraRot.r[2] = cameraAxisZ;
//	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
//	// 転置により逆行列（逆回転）を計算
//	matView = XMMatrixTranspose(matCameraRot);
//
//	// 視点座標に-1を掛けた座標
//	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
//	// カメラの位置からワールド原点へのベクトル（カメラ座標系）
//	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);	// X成分
//	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);	// Y成分
//	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);	// Z成分
//	// 一つのベクトルにまとめる
//	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
//	// ビュー行列に平行移動成分を設定
//	matView.r[3] = translation;
//
//#pragma region 全方向ビルボード行列の計算
//	// ビルボード行列
//	matBillboard.r[0] = cameraAxisX;
//	matBillboard.r[1] = cameraAxisY;
//	matBillboard.r[2] = cameraAxisZ;
//	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
//#pragma region
//
//#pragma region Y軸回りビルボード行列の計算
//	// カメラX軸、Y軸、Z軸
//	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;
//
//	// X軸は共通
//	ybillCameraAxisX = cameraAxisX;
//	// Y軸はワールド座標系のY軸
//	ybillCameraAxisY = XMVector3Normalize(upVector);
//	// Z軸はX軸→Y軸の外積で求まる
//	ybillCameraAxisZ = XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);
//
//	// Y軸回りビルボード行列
//	matBillboardY.r[0] = ybillCameraAxisX;
//	matBillboardY.r[1] = ybillCameraAxisY;
//	matBillboardY.r[2] = ybillCameraAxisZ;
//	matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);
//#pragma endregion
//}
#pragma endregion


