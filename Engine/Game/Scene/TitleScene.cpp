#include "TitleScene.h"
#include "Input.h"
#include "DirectXCommon.h"

void TitleScene::Initialize() {
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

}

void TitleScene::Finalize() {
	delete sprite;
}

void TitleScene::Update() {
	Input* input = Input::GetInstance();
	if (input->PushKey(DIK_SPACE))     // �X�y�[�X�L�[��������Ă�����
	{
		// ��ʃN���A�J���[�̐��l������������
		//clearColor[1] = 1.0f;
	}
}

void TitleScene::Draw() {
	SpriteCommon::GetInstance()->PreDraw();

}
