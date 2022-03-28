#include "TitleScene.h"
#include "Input.h"
#include "DirectXCommon.h"

void TitleScene::Initialize() {
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->LoadTexture(0, L"Resources/house.png");
	for (int i = 0; i < 20; i++)
	{
		int texNum = rand() % 2;

		sprite = Sprite::Create(texNum, { 0,0 }, false, false);

		sprite->SetPosition({ (float)(rand() % 1280),(float)(rand() % 720),0 });

		//sprite->SetRotation((float)(rand() % 360));

		sprite->SetSize({ (float)(rand() % 400), (float)(rand() % 100) });

		sprite->TransferVertexBuffer();

		sprites.push_back(sprite);
		//sprite->SetPosition({ 500,300,0 });

	}

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
	sprite->Update();
}

void TitleScene::Draw() {
	SpriteCommon::GetInstance()->PreDraw();
	for (auto& sprite : sprites)
	{
		sprite->Draw();
	}
}
