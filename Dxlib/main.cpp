#include "DxLib.h"
#include "Vector2.h"

int DrawCircle(Vector2 vec, int r, unsigned int color);




// ウィンドウのタイトルに表示する文字列
const char TITLE[] = "LC1B_00_テラオカヨシヒコ: タイトル";

// ウィンドウ横幅
const int WIN_WIDTH = 1024;

// ウィンドウ縦幅
const int WIN_HEIGHT = 576;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
                   _In_ int nCmdShow) {
	// ウィンドウモードに設定
	ChangeWindowMode(TRUE);

	// ウィンドウサイズを手動では変更させず、
	// かつウィンドウサイズに合わせて拡大できないようにする
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// タイトルを変更
	SetMainWindowText(TITLE);

	// 画面サイズの最大サイズ、カラービット数を設定(モニターの解像度に合わせる)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// 画面サイズを設定(解像度との比率で設定)
	SetWindowSizeExtendRate(1.0);

	// 画面の背景色を設定する
	SetBackgroundColor(0, 0, 64);

	// DXlibの初期化
	if (DxLib_Init() == -1) { return -1; }

	// (ダブルバッファ)描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	// 画像などのリソースデータの変数宣言と読み込み


	// ゲームループで使う変数の宣言
	Vector2 position(10, 100);
	Vector2 velocity(+1.0f, +0.5f);

	// 最新のキーボード情報用
	char keys[256] = {0};

	// 1ループ(フレーム)前のキーボード情報
	char oldkeys[256] = {0};

	// ゲームループ
	while (ProcessMessage()==0&&CheckHitKey(KEY_INPUT_ESCAPE)==0) {
		// 画面クリア
		ClearDrawScreen();
		//---------  ここからプログラムを記述  ----------//

		// 更新処理
		position += velocity;

		// 描画処理
		//DrawCircle(WIN_WIDTH / 2, WIN_HEIGHT / 2, 16, GetColor(128, 255, 128));
		DrawCircle(position, 16, GetColor(128, 255, 128));
		//---------  ここまでにプログラムを記述  ---------//
		// (ダブルバッファ)裏面
		ScreenFlip();
	}
	// Dxライブラリ終了処理
	DxLib_End();

	// 正常終了
	return 0;
}
int DrawCircle(Vector2 vec, int r, unsigned int color) {
	return DrawCircle(static_cast<int>(vec.x), static_cast<int>(vec.y), r, color);
}