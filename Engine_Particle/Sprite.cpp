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


	// �e�N�X�`���ԍ����R�s�[
	texNumber_ = texNumber;

	// �A���J�[�|�C���g���R�s�[
	anchorpoint_ = anchorpoint;

	// ���]�t���O���R�s�[
	isFlipX_ = isFlipX;
	isFlipY_ = isFlipY;

	SspriteCommon = spriteCommon;

	// ���_�f�[�^
	VertexPosUv vertices[4];

	// �w��ԍ��̉摜���ǂݍ��ݍς݂Ȃ�
	if (SspriteCommon->GetTexBuff(texNumber_)) {
		// �e�N�X�`�����擾
		D3D12_RESOURCE_DESC resDesc = SspriteCommon->GetTexBuff(texNumber_)->GetDesc();

		// �X�v���C�g�̑傫�����摜�̉𑜓x�ɍ��킹��
		size_ = { (float)resDesc.Width, (float)resDesc.Height };
		texSize_ = { (float)resDesc.Width, (float)resDesc.Height };
	}

	// ���_�o�b�t�@����
	result = SspriteCommon->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff_));

	// ���_�o�b�t�@�f�[�^�]��
	TransferVertexBuffer();

	// ���_�o�b�t�@�r���[�̍쐬
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeof(vertices);
	vbView_.StrideInBytes = sizeof(vertices[0]);

	// �萔�o�b�t�@�̐���
	result = SspriteCommon->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff_));

	// �萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1); // �F�w��iRGBA�j
	constMap->mat = spriteCommon->GetMatProjection();
	constBuff_->Unmap(0, nullptr);

}

void Sprite::TransferVertexBuffer()
{
	HRESULT result = S_FALSE;

	// ���_�f�[�^
	VertexPosUv vertices[] = {
		//     u     v
		{{}, {0.0f, 1.0f}}, // ����
		{{}, {0.0f, 0.0f}}, // ����
		{{}, {1.0f, 1.0f}}, // �E��
		{{}, {1.0f, 0.0f}}, // �E��
	};

	// �����A����A�E���A�E��
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorpoint_.x) * size_.x;
	float right = (1.0f - anchorpoint_.x) * size_.x;
	float top = (0.0f - anchorpoint_.y) * size_.y;
	float bottom = (1.0f - anchorpoint_.y) * size_.y;

	if (isFlipX_)
	{// ���E����ւ�
		left = -left;
		right = -right;
	}

	if (isFlipY_)
	{// ���E����ւ�
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left, bottom,  0.0f }; // ����
	vertices[LT].pos = { left, top,     0.0f }; // ����
	vertices[RB].pos = { right, bottom, 0.0f }; // �E��
	vertices[RT].pos = { right, top,    0.0f }; // �E��

	// �w��ԍ��̉摜���ǂݍ��ݍς݂Ȃ�
	if (SspriteCommon->GetTexBuff(texNumber_)) {
		// �e�N�X�`�����擾
		D3D12_RESOURCE_DESC resDesc = SspriteCommon->GetTexBuff(texNumber_)->GetDesc();

		float tex_left = texLeftTop_.x / resDesc.Width;
		float tex_right = (texLeftTop_.x + texSize_.x) / resDesc.Width;
		float tex_top = texLeftTop_.y / resDesc.Height;
		float tex_bottom = (texLeftTop_.y + texSize_.y) / resDesc.Height;

		vertices[LB].uv = { tex_left,   tex_bottom }; // ����
		vertices[LT].uv = { tex_left,   tex_top }; // ����
		vertices[RB].uv = { tex_right,  tex_bottom }; // �E��
		vertices[RT].uv = { tex_right,  tex_top }; // �E��
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	vertBuff_->Unmap(0, nullptr);
}

void Sprite::Update()
{
	// ���[���h�s��̍X�V
	matWorld_ = XMMatrixIdentity();
	// Z����]
	matWorld_ *= XMMatrixRotationZ(XMConvertToRadians(rotation_));
	// ���s�ړ�
	matWorld_ *= XMMatrixTranslation(position_.x, position_.y, position_.z);

	// �萔�o�b�t�@�̓]��
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
	// ���_�ƒ����_���W���ړ����A���f
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


	// ���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &vbView_);

	// �萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());

	// �V�F�[�_���\�[�X�r���[���Z�b�g
	SspriteCommon->SetGraphicsRootDescriptorTable(1, texNumber_);
	// �|���S���̕`��i4���_�Ŏl�p�`�j
	cmdList->DrawInstanced(4, 1, 0, 0);
}




#pragma region �ؗj�m�F

//void Sprite::UpdateViewMatrix()
//{
//	// ���_���W
//	XMVECTOR eyePosition = XMLoadFloat3(&eye);
//	// �����_���W
//	XMVECTOR targetPosition = XMLoadFloat3(&target);
//	// �i���́j�����
//	XMVECTOR upVector = XMLoadFloat3(&up);
//
//	// �J����Z���i���������j
//	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
//	// 0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
//	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
//	assert(!XMVector3IsInfinite(cameraAxisZ));
//	assert(!XMVector3Equal(upVector, XMVectorZero()));
//	assert(!XMVector3IsInfinite(upVector));
//	// �x�N�g���𐳋K��
//	cameraAxisZ = XMVector3Normalize(cameraAxisZ);
//
//	// �J������X���i�E�����j
//	XMVECTOR cameraAxisX;
//	// X���͏������Z���̊O�ςŋ��܂�
//	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
//	// �x�N�g���𐳋K��
//	cameraAxisX = XMVector3Normalize(cameraAxisX);
//
//	// �J������Y���i������j
//	XMVECTOR cameraAxisY;
//	// Y����Z����X���̊O�ςŋ��܂�
//	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
//
//	// �����܂łŒ�������3�����̃x�N�g��������
//	//�i���[���h���W�n�ł̃J�����̉E�����A������A�O�����j	
//
//	// �J������]�s��
//	XMMATRIX matCameraRot;
//	// �J�������W�n�����[���h���W�n�̕ϊ��s��
//	matCameraRot.r[0] = cameraAxisX;
//	matCameraRot.r[1] = cameraAxisY;
//	matCameraRot.r[2] = cameraAxisZ;
//	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
//	// �]�u�ɂ��t�s��i�t��]�j���v�Z
//	matView = XMMatrixTranspose(matCameraRot);
//
//	// ���_���W��-1���|�������W
//	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
//	// �J�����̈ʒu���烏�[���h���_�ւ̃x�N�g���i�J�������W�n�j
//	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);	// X����
//	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);	// Y����
//	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);	// Z����
//	// ��̃x�N�g���ɂ܂Ƃ߂�
//	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
//	// �r���[�s��ɕ��s�ړ�������ݒ�
//	matView.r[3] = translation;
//
//#pragma region �S�����r���{�[�h�s��̌v�Z
//	// �r���{�[�h�s��
//	matBillboard.r[0] = cameraAxisX;
//	matBillboard.r[1] = cameraAxisY;
//	matBillboard.r[2] = cameraAxisZ;
//	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
//#pragma region
//
//#pragma region Y�����r���{�[�h�s��̌v�Z
//	// �J����X���AY���AZ��
//	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;
//
//	// X���͋���
//	ybillCameraAxisX = cameraAxisX;
//	// Y���̓��[���h���W�n��Y��
//	ybillCameraAxisY = XMVector3Normalize(upVector);
//	// Z����X����Y���̊O�ςŋ��܂�
//	ybillCameraAxisZ = XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);
//
//	// Y�����r���{�[�h�s��
//	matBillboardY.r[0] = ybillCameraAxisX;
//	matBillboardY.r[1] = ybillCameraAxisY;
//	matBillboardY.r[2] = ybillCameraAxisZ;
//	matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);
//#pragma endregion
//}
#pragma endregion

