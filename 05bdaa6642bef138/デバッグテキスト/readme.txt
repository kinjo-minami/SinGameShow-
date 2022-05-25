<デバッグテキストを使うのに必要なもの>

// 初期化
DebugText* debugText = nullptr;
debugText = new DebugText();
const int debugTextTexNumber = 0;
spriteCommon->LoadTexture(debugTextTexNumber, L"Resources/debugfont3.png");
debugText->Initialize(spriteCommon, debugTextTexNumber);

// 必要な変数
char str[100] = { 0 };
int score = 0;
XMFLOAT2 scorePos = { 0.0f, 0.0f };

// 更新

// ゲームプレイ画面に入ったらスコアの位置を(0, 0)にする（タイトルに戻る場合に入れる）
scorePos = { 0.0f, 0.0f }

// ゲームプレイ画面、リザルト画面に入れる
sprintf_s(str, "SCORE:%d", (int)score);
debugText->Print(str, scorePos.x, scorePos.y, 1.0f);

// リザルト画面に入ったらスコアの位置を(340, 220)にする
scorePos = { 340.0f, 220.0f }

// 描画

// ゲームプレイ画面、リザルト画面に入れる
debugText->DrawAll();

// 解放
delete debugText;

<最後に>

.hと.cppを入れ忘れるようなばかなことはするなよ！