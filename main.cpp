#include "DxLib.h"

// ウィンドウのタイトルに表示する文字列
const char TITLE[] = "LC1B_31_ワタナベライキ:タイトル";

// ウィンドウ横幅
const int WIN_WIDTH = 896;

// ウィンドウ縦幅
const int WIN_HEIGHT = 896;

float easeOutBounce(float x) {
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (x < 1 / d1) {
		return n1 * x * x;
	}
	else if (x < 2 / d1) {
		return n1 * (x -= 1.5 / d1) * x + 0.75;
	}
	else if (x < 2.5 / d1) {
		return n1 * (x -= 2.25 / d1) * x + 0.9375;
	}
	else {
		return n1 * (x -= 2.625 / d1) * x + 0.984375;
	}
}
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
	SetBackgroundColor(0x00, 0x00, 0x00);

	// DXlibの初期化
	if (DxLib_Init() == -1) { return -1; }

	// (ダブルバッファ)描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	// 画像などのリソースデータの変数宣言と読み込み
	int titlegh = LoadGraph("title2.png");
	int backgh = LoadGraph("back.png");
	int space[6];
	LoadDivGraph("space.png", 6, 6, 1, 896, 896, space);
	// ゲームループで使う変数の宣言

	//タイトル
	int spaceMove = 0;
	int ismove = 1;
	float frame = 0.0f;
	float endframe = 250.0f;
	float start = -386.0f;
	float end = 100.0f;
	int titleX = 56;
	int titleY = 0;

	//シーン
	int scene = 0;
	//光変更
	int changeMode = 0;
	int changeBack = 0;
	int changeCount = 0;
	int alpha = 200;
	int changeX = WIN_WIDTH / 2;
	int changeY = WIN_HEIGHT / 2;
	int changeR = 0;
	int changeCoolTime = 0;
	int isChange = 0;
	// 最新のキーボード情報用
	char keys[256] = { 0 };

	// 1ループ(フレーム)前のキーボード情報
	char oldkeys[256] = { 0 };

	// ゲームループ
	while (true) {
		// 最新のキーボード情報だったものは1フレーム前のキーボード情報として保存
		for (int i = 0; i < 256; i++) {
			oldkeys[i] = keys[i];
		}
		// 最新のキーボード情報を取得
		GetHitKeyStateAll(keys);

		// 画面クリア
		ClearDrawScreen();
		//---------  ここからプログラムを記述  ----------//

		// 更新処理
		switch (scene) {
		case 0:
			if (keys[KEY_INPUT_SPACE] == 1 && oldkeys[KEY_INPUT_SPACE] == 0) {
				if (changeCount == 0) {
					changeCount = 1;
				}
			}

			if (frame == endframe) {
				ismove = 0;
			}
			if (ismove == 1) {
				frame += 1;
			}
			if (spaceMove < 50) {
				spaceMove += 1;
			}
			if (spaceMove == 50) {
				spaceMove = 0;
			}
			//イージング
			titleY = start + (end - start) * easeOutBounce(frame / endframe);

			//だんだん大きくなる
			if (changeCount == 1 && changeBack == 0) {
				changeR += 10;
			}
			
			//戻る
			if (changeR >= 1000 && changeBack == 0) {
				//ここでシーン切り替え

				changeBack = 1;
				scene = 1;
				////////////////////
			}
			break;
		case 1:
			if (changeBack == 1) {
				changeR -= 10;
			}
			if (changeR <= 0 && changeBack == 1) {
			changeBack = 0;
			changeCount = 0;
			changeR = 0;
			isChange = 1;
		}

			break;
		}
		
		// 描画処理
		switch (scene) {
		case 0:
			DrawGraph(0, 0, backgh, true);
			DrawGraph(titleX, titleY, titlegh, true);
			DrawGraph(0, 0, space[spaceMove / 10], true);
			if (changeCount == 1) {
				DrawCircle(changeX, changeY, changeR, GetColor(255, 255, 255), true);
			}
			break;
		case 1:
			DrawFormatString(500,500,GetColor(255,255,255),"ステージ選択");
			if (changeCount == 1) {
				DrawCircle(changeX, changeY, changeR, GetColor(255, 255, 255), true);
			}
		}
		//---------  ここまでにプログラムを記述  ---------//
		// (ダブルバッファ)裏面
		ScreenFlip();

		// 20ミリ秒待機(疑似60FPS)
		WaitTimer(20);

		// Windowsシステムからくる情報を処理する
		if (ProcessMessage() == -1) {
			break;
		}

		// ESCキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			break;
		}
	}
	// Dxライブラリ終了処理
	DxLib_End();

	// 正常終了
	return 0;
}
