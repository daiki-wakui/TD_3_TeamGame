#include "DxLib.h"
#include "Player.h"
#include "Laser.h"
#include "Map.h"
#include"Colision.h"
#include "UI.h"
#include "Sturct.h"


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
	{
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
	SetBackgroundColor(120, 120, 120);

	// DXlibの初期化
	if (DxLib_Init() == -1) { return -1; }

	// (ダブルバッファ)描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	// 画像などのリソースデータの変数宣言と読み込み
	int backHandle = LoadGraph("background.png");

	LoadDivGraph("block.png", 16, 4, 4, 32, 32, map.chipBlock, true);
	map.crystalPic = LoadGraph("crystal.png");
	int colorPalette[3];
	LoadDivGraph("color_pallette.png", 3, 3, 1, 64, 64, colorPalette, true);
	int gauss = MakeScreen(WIN_WIDTH, WIN_HEIGHT, true);
	
	
	// ゲームループで使う変数の宣言
	int scene = 0;
	int isClear = 0;
	int reset = 0;
	int isSceneChange = 0;

	int stage = 0;
	int stageMax = 4;

	int time = 0;
	int ms = 0;
	int second = 0;
	int minute = 0;

	int blurHandle = MakeScreen(WIN_WIDTH, WIN_HEIGHT);
	//------------------コントローラー変数宣言-----------------------//
	XINPUT_STATE pad;
	for (int i = 0; i < 16; i++) {
		pad.Buttons[i] = 0;
		}
	pad.ThumbLX = 0;
	pad.ThumbLY = 0;
	pad.ThumbRX = 0;
	pad.ThumbRY = 0;
	XINPUT_STATE padOld;
	SetJoypadDeadZone(DX_INPUT_PAD1, 1);

	//------------------プレイヤー初期化--------------//
	Player* player = new Player();

	//-------------------レーザー初期化--------------//
	Laser* redLaser = new Laser(map, R_LASER);
	Laser* greenLaser = new Laser(map, G_LASER);
	Laser* blueLaser = new Laser(map, B_LASER);

	//--------------ポーズ画面インスタンス生成----------//
	UI* ui = new UI;

	// 最新のキーボード情報用
	char keys[256] = { 0 };

	// 1ループ(フレーム)前のキーボード情報
	char oldkeys[256] = { 0 };

	// ゲームループ
	while (1)
		{
		// 最新のキーボード情報だったものは1フレーム前のキーボード情報として保存
		for (int i = 0; i < 256; ++i)
			{
			oldkeys[i] = keys[i];
			}
		// 最新のキーボード情報を取得
		GetHitKeyStateAll(keys);

		// 画面クリア
		ClearDrawScreen();
		//---------  ここからプログラムを記述  ----------//

		//--------------------------------------------------更新処理----------------------------------------------------------//

		//コントローラー判定
		for (int i = 0; i < 16; i++) {
			padOld.Buttons[i] = pad.Buttons[i];
			}
		padOld.ThumbLX = pad.ThumbLX;
		padOld.ThumbLY = pad.ThumbLY;
		padOld.ThumbRX = pad.ThumbRX;
		padOld.ThumbRY = pad.ThumbRY;
		GetJoypadXInputState(DX_INPUT_PAD1, &pad);


		//シーン
		switch (scene) {
				case 0://タイトル画面---------------------------------------------------
					if (ui->isChange == false) {
						ui->returnTitle = false;
					}
					//ステージ選択へ
					if (pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0) {

						if (ui->isChange == false) {
							ui->isChange = true;
							ui->isIn = true;
						}
					}

					if (ui->isChange == true) {
						ui->SceneTransition();
					}

					if (ui->isIn == false && ui->isChange == true && ui->returnTitle == false) {
						scene = 1;
					}

					break;

				case 1://ステージ選択画面-----------------------------------------------

					if (ui->isChange == true) {
						ui->SceneTransition();
					}

					//ステージ選択
					if (pad.ThumbLX > 9000 && padOld.ThumbLX < 9000) {
						if (map.stage != stageMax) {
							map.stage++;
						}
					}
					else if (pad.ThumbLX < -9000 && padOld.ThumbLX > -9000) {
						if (map.stage != 0) {
							map.stage--;
						}
					}

					//ゲーム画面へ
					if (pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0) {
						if (ui->isChange == false) {
							ui->isChange = true;
							ui->isIn = true;
						}
					}

					if (ui->isChange == true) {
						ui->SceneTransition();
					}

					if (ui->fade >= 225) {
						scene = 2;
						reset = 1;
					}

					break;

				case 2://ゲーム画面------------------------------------------------------

					if (ui->isChange == true) {
						ui->SceneTransition();
					}

					if (isClear == 0) {

						time += 20;
						ms = time / 10;

						if (time >= 1000) {
							time = 0;
							second++;
							ms = 0;
						}

						if (ms >= 100) {
							second++;
							ms = 0;
						}
						if (second >= 60) {
							minute++;
							second = 0;
						}

						//ポーズ画面へ
						if (pad.Buttons[XINPUT_BUTTON_Y] == 1 && padOld.Buttons[XINPUT_BUTTON_Y] == 0) {
							scene = 3;
							DeleteGraph(gauss);
						}

						////////////////プレイヤー///////////////
						//移動
						player->isMove = 1;
						if (redLaser->isAngle == 1 || blueLaser->isAngle == 1 || greenLaser->isAngle == 1) {
							player->isMove = 0;
						}
						else {
							player->isMove = 1;
						}
						player->Move(pad, padOld);

						///////////////レーザー////////////////
						redLaser->Angle(pad, padOld, player->transform);
						greenLaser->Angle(pad, padOld, player->transform);
						blueLaser->Angle(pad, padOld, player->transform);

						//クリア判定
						if (map.isDraw[RED] == 1 && map.isDraw[GREEN] == 1 && map.isDraw[BLUE] == 1) {
							isClear = 1;
							}
						}
					//クリア後
					else if (isClear == 1) {
						if (pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0) {
							scene = 0;
						}
					}

					if (player->transform.posY >= WIN_HEIGHT) {
						scene = 4;
					}

					ui->magnification = 0;
					break;

				case 3://ポーズ画面------------------------------------------------------
				
					if (ui->isChange == true) {
						ui->SceneTransition();
					}

					ui->PauseUpdate(pad, padOld, scene, reset);

					break;

				case 4:
					if (pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0) {
						player->Reset();
						delete redLaser;
						delete greenLaser;
						delete blueLaser;
						player = new Player();
						redLaser = new Laser(map, R_LASER);
						greenLaser = new Laser(map, G_LASER);
						blueLaser = new Laser(map, B_LASER);
						scene = 2;
					}

					break;
			}

		//初期化
		if (reset == 1) {
			reset = 0;
			player->Reset();
			redLaser = new Laser(map, R_LASER);
			greenLaser = new Laser(map, G_LASER);
			blueLaser = new Laser(map, B_LASER);
			for (int i = 2; i < 6; i++) {
				map.isDraw[i] = 0;
			}
			isClear = 0;
			time = 0;
			second = 0;
			ms = 0;
		}

		//-------------------------------------------------描画処理------------------------------------------------------------------//

		//シーン
		switch (scene) {
				case 0://タイトル
					if (ui->isChange == true) {
						ui->DrawFadeInOut();
					}
					
				//デバッグ用
					DrawFormatString(500, 430, GetColor(255, 255, 255), "タイトル画面");
					DrawFormatString(500, 530, GetColor(255, 255, 255), "Bでステージ選択へ");				

					break;

				case 1://ステージ選択画面---------------------------------------------------
					if (ui->isChange == true) {
						ui->DrawFadeInOut();
					}
				//デバッグ用
					DrawFormatString(500, 430, GetColor(255, 255, 255), "ステージ選択画面");
					DrawFormatString(500, 500, GetColor(255, 255, 255), "Lスティックで選択");
					DrawFormatString(500, 530, GetColor(255, 255, 255), "Bでステージ%dへ", map.stage + 1);
					break;

				case 2://ゲーム画面----------------------------------------------------------

					DrawGraph(0, 0, backHandle, true);
					redLaser->Draw();
					greenLaser->Draw();
					blueLaser->Draw();
					Map();
					player->Draw();
					ui->ButtonDraw(pad,padOld,player->transform);
					ui->drawColorPalette(1200, 60, colorPalette);

					if (pad.Buttons[XINPUT_BUTTON_Y] == 1 && padOld.Buttons[XINPUT_BUTTON_Y] == 0) {
						GetDrawScreenGraph(0, 0, WIN_WIDTH, WIN_HEIGHT, gauss);
					}
					DrawFormatString(1300, 100, GetColor(220, 220, 220), "%02d:%02d:%02d", minute, second, ms);

					if (isClear == 1) {
						SetFontSize(64);
						DrawFormatString(650, 400, GetColor(220, 220, 220), "CLEAR");
						DrawFormatString(500, 530, GetColor(255, 255, 255), "Bでタイトル画面へ");
						SetFontSize(16);
					}

					if (ui->isChange == true) {
						ui->DrawFadeInOut();
					}
					//デバッグ用
					DrawFormatString(20, 800, GetColor(255, 255, 255), "ゲーム画面　ステージ%d", map.stage);
					DrawFormatString(20, 820, GetColor(255, 255, 255), "Lスティック:移動");
					DrawFormatString(20, 840, GetColor(255, 255, 255), "RT:ダッシュ");
					DrawFormatString(20, 860, GetColor(255, 255, 255), "LT長押し:グライダー展開");
					DrawFormatString(20, 880, GetColor(255, 255, 255), "A:ジャンプ");
					DrawFormatString(20, 900, GetColor(255, 255, 255), "レーザーブロックの近くで B:モード切替 起動/解除");
					DrawFormatString(20, 920, GetColor(255, 255, 255), "角度変更モード中 Rスティック：レーザー角度変更");
					DrawFormatString(20, 940, GetColor(255, 255, 255), "Y:ポーズ");
					DrawFormatString(1000, 20, GetColor(255, 255, 255), "LX %d\nLY %d", pad.ThumbLX, pad.ThumbLY);
					DrawFormatString(1000, 60, GetColor(255, 255, 255), "RX %d\nRY %d", pad.ThumbRX, pad.ThumbRY);
					DrawFormatString(1000, 100, GetColor(255, 255, 255), "%lf", redLaser->theta);
					if (player->isMove == 1) {
						DrawFormatString(1250, 920, GetColor(255, 255, 255), "今のモード 移動");
					}
					else {
						DrawFormatString(1250, 920, GetColor(255, 255, 255), "今のモード 角度変更");
						DrawFormatString(20, 920, GetColor(255, 0, 0), "角度変更モード中 Rスティック：レーザー角度変更");
					}
					if (player->isJump == 1) {
						DrawFormatString(20, 880, GetColor(255, 0, 0), "A:ジャンプ");
					}
					if (pad.ThumbLX > 4500 || pad.ThumbLX < -4500) {
						DrawFormatString(20, 820, GetColor(255, 0, 0), "Lスティック:移動");
					}
					if (pad.LeftTrigger > 150) {
						DrawFormatString(20, 860, GetColor(255, 0, 0), "LT長押し:グライダー展開");
					}
					if (pad.RightTrigger > 150) {
						DrawFormatString(20, 840, GetColor(255, 0, 0), "RT:ダッシュ");
					}

					break;

				case 3://ポーズ画面-------------------------------------------------------------
					////後ろでゲーム画面を描画////
					DrawGraph(0, 0, backHandle, true);
					redLaser->Draw();
					greenLaser->Draw();
					blueLaser->Draw();
					Map();
					player->Draw();
					
					GetDrawScreenGraph(0, 0, WIN_WIDTH, WIN_HEIGHT, blurHandle);
					GraphFilter(blurHandle, DX_GRAPH_FILTER_GAUSS, 32, 1200);
					DrawGraph(0, 0, blurHandle, true);
					/////-----------------////
					ui->drawColorPalette(1200, 60, colorPalette);
					ui->PauseDraw();

					if (ui->isChange == true) {
						ui->DrawFadeInOut();
					}

					DrawFormatString(1000, 20, GetColor(255, 255, 255), "LX %d\nLY %d", pad.ThumbLX, pad.ThumbLY);
					DrawFormatString(1000, 60, GetColor(255, 255, 255), "RX %d\nRY %d", pad.ThumbRX, pad.ThumbRY);
					DrawFormatString(1000, 100, GetColor(255, 255, 255), "%lf", redLaser->theta);
					break;

				case 4:

					DrawFormatString(700, 480, GetColor(255, 255, 255), "GameOver\nBでリトライ");
		}
		/*DrawFormatString(1000, 500, GetColor(255, 255, 255), "state %d", player->state);
		DrawFormatString(1000, 520, GetColor(255, 255, 255), "a %d", player->idleTimer);
		DrawFormatString(700, 480, GetColor(255, 255, 255), "scene %d",scene);
		DrawFormatString(500, 550, GetColor(255, 255, 255), "fade %d", ui->fade);
		DrawFormatString(500, 570, GetColor(255, 255, 255), "re %d", ui->returnTitle);
		DrawFormatString(500, 590, GetColor(255, 255, 255), "gDraw3 %d", greenLaser->isDraw[3]);
		DrawFormatString(500, 610, GetColor(255, 255, 255), "gDraw2 %d", greenLaser->isDraw[2]);*/
		//---------  ここまでにプログラムを記述  ---------//
		// (ダブルバッファ)裏面
		ScreenFlip();

		// 20ミリ秒待機(疑似60FPS)
		WaitTimer(20);

		// Windowsシステムからくる情報を処理する
		if (ProcessMessage() == -1)
			{
			break;
		}

		// ESCキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			{
			break;
		}
	}
	// Dxライブラリ終了処理
	DxLib_End();

	// 正常終了
	return 0;
	}