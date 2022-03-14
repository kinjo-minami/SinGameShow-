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
	if (input->PushKey(DIK_SPACE))     // スペースキーが押されていたら
	{
		// 画面クリアカラーの数値を書き換える
		//clearColor[1] = 1.0f;
	}
}

void TitleScene::Draw() {
	SpriteCommon::GetInstance()->PreDraw();

}
