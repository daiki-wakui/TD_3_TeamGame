#include "DxLib.h"
#include "Player.h"
#include "Laser.h"
#include "Map.h"
#include"Colision.h"
#include "UI.h"
#include "Sturct.h"
#include "Tutorial.h"
#include<math.h>
#include "SE.h"


double easeOutCirc(double x);

void BgColor();

class Fps {
	int mStartTime;         //測定開始時刻
	int mCount;             //カウンタ
	float mFps;             //fps
	static const int N = 45;//平均を取るサンプル数
	static const int FPS = 45;    //設定したFPS

public:
	Fps() {
		mStartTime = 0;
		mCount = 0;
		mFps = 0;
		}

	bool Update() {
		if (mCount == 0) { //1フレーム目なら時刻を記憶
			mStartTime = GetNowCount();
			}
		if (mCount == N) { //60フレーム目なら平均を計算する
			int t = GetNowCount();
			mFps = 1000.f / ((t - mStartTime) / (float)N);
			mCount = 0;
			mStartTime = t;
			}
		mCount++;
		return true;
		}

	void Wait() {
		int tookTime = GetNowCount() - mStartTime;    //かかった時間
		int waitTime = mCount * 1000 / FPS - tookTime;    //待つべき時間
		if (waitTime > 0) {
			WaitTimer(waitTime);    //待機
			}
		}
	};



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
	Fps fps;
	// (ダブルバッファ)描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	// 画像などのリソースデータの変数宣言と読み込み
	BgColor();

	int titlePic = LoadGraph("Resources/title_screen.png");
	int titleCrystal = LoadGraph("Resources/title_screen-crystal.png");
	int titleTextPic[4];
	LoadDivGraph("Resources/title_screen-text.png", 4, 4, 1, 1405, 960, titleTextPic, true);

	LoadDivGraph("Resources/block.png", 16, 4, 4, 32, 32, map.chipBlock, true);
	LoadDivGraph("Resources/crystal.png", 4, 4, 1, 192, 256, map.crystalPic, true);
	int colorPalette[3];
	LoadDivGraph("Resources/color_pallette.png", 3, 3, 1, 64, 64, colorPalette, true);
	int vignette = LoadGraph("Resources/vignette.png");
	int gauss = MakeScreen(WIN_WIDTH, WIN_HEIGHT, true);


	// ゲームループで使う変数の宣言
	int scene = 0;
	int isClear = 0;
	int reset = 0;
	int isSceneChange = 0;

	int titleAlpha = 0;
	int titleTextAlpha = 0;
	int titleSelect = 0;

	int stage = 0;
	int stageMax = 4;

	int time = 0;
	int ms = 0;
	int second = 0;
	int minute = 0;

	int rAlpha = 0, gAlpha = 0, bAlpha = 0;
	//-----------------------------------------//


	//-----ステージ選択
	int stageSelectPic[5];
	LoadDivGraph("Resources/stage_select.png", 5, 5, 1, 1408, 960, stageSelectPic, true);
	int isChange = 0;

	//-----クリア------
	int light = LoadGraph("Resources/light.png", true);
	int clearPic[3];
	LoadDivGraph("Resources/stageclear.png", 3, 3, 1, 800, 510, clearPic, true);
	int clearPicNum = 0;
	float ex = 0;
	int count = 0;
	int clearAlpha = 255, clearAlpha2 = 0;
	double flame = 0, flameMax = 65;
	double flamePerFlamemax;

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
	//--------------チュートリアルインスタンス生成------//
	Tutorial* tutorial = new Tutorial;

	// 最新のキーボード情報用
	char keys[256] = { 0 };

	// 1ループ(フレーム)前のキーボード情報
	char oldkeys[256] = { 0 };

	// ゲームループ
	while (1)
		{
		fps.Update();    //更新
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


		double dead = atan2(pad.ThumbLY, pad.ThumbLX);
		int deadX = cos(dead) * 25000;
		int deadY = sin(dead) * -25000;

		//シーン
		switch (scene) {
				case 0://タイトル画面---------------------------------------------------
					if (CheckSoundMem(se.mainBGM) == 0) {
						PlaySoundMem(se.mainBGM, DX_PLAYTYPE_LOOP, true);
					}


					if (ui->isChange == false) {
						ui->returnTitle = false;
						}
					//----アルファ演出----//
					titleAlpha += 3;
					if (titleAlpha >= 180) {
						titleTextAlpha += 10;
						}

					if (titleAlpha >= 255) {
						titleAlpha = 255;
						}
					if (titleTextAlpha >= 255) {
						titleTextAlpha = 255;
						}
					//------------------//

					//ステージ選択へ
					if (titleTextAlpha == 255) {
						if (pad.ThumbLY > 9000 && padOld.ThumbLY < 9000) {

							if (titleSelect != 0) {
								//PlaySoundMem(menuSelectSE, DX_PLAYTYPE_BACK, true);
								titleSelect--;
								}
							}
						else if (pad.ThumbLY < -9000 && padOld.ThumbLY>-9000) {
							if (titleSelect != 3) {
								//PlaySoundMem(menuSelectSE, DX_PLAYTYPE_BACK, true);
								titleSelect++;
								}
							}
						}

					if (pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0) {
						//PlaySoundMem(menuPickSE, DX_PLAYTYPE_BACK, true);
						//アルファスキップ
						if (titleTextAlpha <= 255) {
							titleAlpha = 255;
							titleTextAlpha = 255;
							}

						if (titleSelect == 3) {
							ui->isContorolMethod = true;	//操作方法画面を開く
							}

						if (ui->isChange == false && titleSelect != 0 && titleSelect != 3) {
							ui->isChange = true;
							ui->isIn = true;
							}
						}

					//Yを押したら操作方法画面
					if (pad.Buttons[XINPUT_BUTTON_Y] == 1 && padOld.Buttons[XINPUT_BUTTON_Y] == 0 &&
						ui->isContorolMethod == true) {

						ui->isContorolMethod = false;
						}

					//操作方法画面を開いている
					if (ui->isContorolMethod == true) {
						ui->ContorolMagnification += 0.2;

						if (ui->ContorolMagnification > 1.4) {
							ui->ContorolMagnification = 1.4;
							}
						}
					//操作方法画面を閉じる
					else if (ui->isContorolMethod == false) {
						ui->ContorolMagnification -= 0.2;

						if (ui->ContorolMagnification < 0.0) {
							ui->ContorolMagnification = 0.0;
							}
						}

					if (ui->isChange == true) {
						ui->SceneTransition();
						}

					if (ui->isIn == false && ui->isChange == true && ui->returnTitle == false) {
						if (titleSelect == 1) {
							//StopSoundMem(titleBGM);
							map.stage = 0;
							scene = 2;
							reset = 1;
							}
						else if (titleSelect == 2) {
							scene = 1;
							}
						}

					break;

				case 1://ステージ選択画面-----------------------------------------------
					/*if (CheckSoundMem(titleBGM) == 0) {
						PlaySoundMem(titleBGM, DX_PLAYTYPE_LOOP, true);
					}*/

					if (ui->isChange == true) {
						ui->SceneTransition();
					}

					//ステージ選択
					if (abs(pad.ThumbLX) > abs(deadX) && abs(pad.ThumbLY) > abs(deadY)) {
						if (isChange == 0) {
							if (map.stage == 0) {
								if (pad.ThumbLX > 25000 && padOld.ThumbLX<25000 && pad.ThumbLY > -10000 && padOld.ThumbLY < 10000) {
									map.stage = 1;
									isChange = 1;
									}
								else if (pad.ThumbLY< -25000 && padOld.ThumbLY > -25000 && pad.ThumbLX > -10000 && padOld.ThumbLX < 10000) {
									map.stage = 2;
									isChange = 1;
									}
								else if (dead < -0.5 && dead > -1) {
									map.stage = 4;
									isChange = 1;
									}
								}
							else if (map.stage == 1) {
								if (pad.ThumbLX < -25000 && padOld.ThumbLX > -25000 && pad.ThumbLY > -10000 && padOld.ThumbLY < 10000) {
									map.stage = 0;
									isChange = 1;
									}
								else if (pad.ThumbLY<-20000 && padOld.ThumbLY > -25000 && pad.ThumbLX > -10000 && padOld.ThumbLX < 10000) {
									map.stage = 3;
									isChange = 1;
									}
								else if (dead < -2 && dead > -2.5) {
									map.stage = 4;
									isChange = 1;
									}
								}
							else if (map.stage == 2) {
								if (pad.ThumbLX > 25000 && padOld.ThumbLX < 25000 && pad.ThumbLY > -10000 && padOld.ThumbLY < 10000) {
									map.stage = 3;
									isChange = 1;
									}
								else if (pad.ThumbLY > 25000 && padOld.ThumbLY < 25000 && pad.ThumbLX > -10000 && padOld.ThumbLX < 10000) {
									map.stage = 0;
									isChange = 1;
									}
								else if (dead < 1 && dead > 0.5) {
									map.stage = 4;
									isChange = 1;
									}
								}
							else if (map.stage == 3) {
								if (pad.ThumbLX < -25000 && padOld.ThumbLX > -25000 && pad.ThumbLY > -10000 && padOld.ThumbLY < 10000) {
									map.stage = 2;
									isChange = 1;
									}
								else if (pad.ThumbLY > 25000 && padOld.ThumbLY < 25000 && pad.ThumbLX > -10000 && padOld.ThumbLX < 10000) {
									map.stage = 1;
									isChange = 1;
									}
								else if (dead < 2.5 && dead > 2) {
									map.stage = 4;
									isChange = 1;
									}
								}
							else if (map.stage == 4) {
								if (dead < 3.1 && dead > 1.5) {
									map.stage = 0;
									isChange = 1;
									}
								else if (dead < 1.5 && dead > 0) {
									map.stage = 1;
									isChange = 1;
									}
								else if (dead < -1.5 && dead > -3.1) {
									map.stage = 2;
									isChange = 1;
									}
								else if (dead < 0 && dead > -1.5) {
									map.stage = 3;
									isChange = 1;
									}
								}
							}
						}
					else {
						isChange = 0;
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

					tutorial->Update(pad, padOld, scene);

					if (ui->fade >= 225) {
						//StopSoundMem(titleBGM);
						scene = 2;
						reset = 1;
					}

					break;

				case 2://ゲーム画面------------------------------------------------------
					/*if (CheckSoundMem(stageBGM) == 0) {
						PlaySoundMem(stageBGM, DX_PLAYTYPE_LOOP, true);
					}*/

					if (ui->isChange == true) {
						ui->SceneTransition();
						}

					if (tutorial->isGameTutorial == true) {
						tutorial->Update(pad, padOld, scene);
						}

					if (map.stage != 0 && map.stage != 1) {
						tutorial->isGameTutorial = false;
						}

					if (isClear == 0 && tutorial->isGameTutorial == false) {

						if (map.isDraw[RED] == true && map.isDraw[GREEN] == true) {

							if (player->isMove == 0 && tutorial->Page == 4) {
								tutorial->Count = 0;
								tutorial->isGameTutorial = true;
								}
							}

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
							//StopSoundMem(stageBGM);
							isClear = 1;
						}
					}

					//---------クリア後----------
					else if (isClear == 1) {
						if (map.stage == stageMax) {
							clearPicNum = 2;
							}
						//クリア演出
						clearAlpha -= 2;
						if (clearAlpha < 100) {
							if (clearAlpha2 <= 216) {
								clearAlpha2 += 4;
								}
							}
						if (count == 0) {
							if (map.alpha[CYAN] > 55 && map.alpha[MAGENTA] > 55 && map.alpha[YELLOW] > 55) {
								//サイズと透過度変化
								if (flame < flameMax) {
									flame++;
									}
								flamePerFlamemax = flame / flameMax;
								ex = 40 * easeOutCirc(flamePerFlamemax);
								if (clearAlpha <= 50) {
									count = 1;
									}
								}
							}

						//クリア後選択
						else if (count == 1) {
							if (map.stage != stageMax) {
								if (pad.ThumbLY > 9000 && padOld.ThumbLY < 9000) {
									clearPicNum = 0;
									}
								if (pad.ThumbLY < -9000 && padOld.ThumbLY > -9000) {
									clearPicNum = 1;
									}
								}

							if (pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0) {
								if (clearPicNum == 0) {
									map.stage++;
									reset = 1;
									}
								else if (clearPicNum == 1 || clearPicNum == 2) {
									scene = 0;
									}
								}
							}
						}

						if (player->transform.posY >= WIN_HEIGHT) {
							ui->isChange = true;
							ui->isIn = true;

							if (ui->fade >= 245) {
								reset = 1;
							}
						}

					ui->magnification = 0;
					break;

				case 3://ポーズ画面------------------------------------------------------

					if (ui->isChange == true) {
						ui->SceneTransition();
					}

					ui->PauseUpdate(pad, padOld, scene, reset, titleAlpha, titleTextAlpha);

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

				case 5:
					break;
			}

		//初期化
		if (reset == 1) {
			reset = 0;
			player->Reset();
			tutorial = new Tutorial;
			redLaser = new Laser(map, R_LASER);
			greenLaser = new Laser(map, G_LASER);
			blueLaser = new Laser(map, B_LASER);
			for (int i = 2; i < 8; i++) {
				map.isDraw[i] = 0;
				map.alpha[i] = 0;
				}
			isClear = 0;
			time = 0;
			second = 0;
			ms = 0;
			ui = new UI;
			count = 0;
			ex = 0;
			clearAlpha = 255;
			clearAlpha2 = 0;
			flame = 0;
			clearPicNum = 0;
			}

		//-------------------------------------------------描画処理------------------------------------------------------------------//

		//シーン
		switch (scene) {
				case 0://タイトル

					DrawGraph(0, 0, titlePic, true);
					SetDrawBlendMode(DX_BLENDMODE_ADD, titleAlpha);
					DrawGraph(1408 / 2, 0, titleCrystal, true);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
					SetDrawBlendMode(DX_BLENDMODE_ADD, titleTextAlpha);

					if (titleSelect == 0) {
						DrawGraph(0, 0, titleTextPic[0], true);
						}
					else if (titleSelect == 1) {
						DrawGraph(0, 0, titleTextPic[1], true);
						}
					else if (titleSelect == 2) {
						DrawGraph(0, 0, titleTextPic[2], true);
						}
					else if (titleSelect == 3) {
						DrawGraph(0, 0, titleTextPic[3], true);
						}

					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

					////----操作方法の画面描画----////
					DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, ui->ContorolMagnification, 0.0, ui->ControlImage, true);
					DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, ui->ContorolMagnification, 0.0, ui->xBoxImage, true);

					if (ui->isChange == true) {
						ui->DrawFadeInOut();
						}

					//デバッグ用
					DrawFormatString(0, 20, GetColor(255, 255, 255), "タイトルクリスタルアルファ %d", titleAlpha);
					DrawFormatString(0, 40, GetColor(255, 255, 255), "タイトルテキストアルファ %d", titleTextAlpha);
					DrawFormatString(500, 430, GetColor(255, 255, 255), "タイトル画面");
					DrawFormatString(500, 530, GetColor(255, 255, 255), "Bでステージ選択へ");

					break;

				case 1://ステージ選択画面---------------------------------------------------
					if (ui->isChange == true) {
						ui->DrawFadeInOut();
						}
					DrawGraph(0, 0, stageSelectPic[map.stage], true);

					break;

				case 2://ゲーム画面----------------------------------------------------------

					DrawGraph(0, 0, map.bg[NONE], true);

					SetDrawBlendMode(DX_BLENDMODE_ADD, map.alpha[RED]);
					DrawGraph(0, 0, map.bg[RED], true);

					SetDrawBlendMode(DX_BLENDMODE_ADD, map.alpha[GREEN]);
					DrawGraph(0, 0, map.bg[GREEN], true);

					SetDrawBlendMode(DX_BLENDMODE_ADD, map.alpha[BLUE]);
					DrawGraph(0, 0, map.bg[BLUE], true);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

					if (clearAlpha < 140) {
						DrawGraph(0, 0, map.bg[NORMAL], true);
						}

					redLaser->Draw();
					greenLaser->Draw();
					blueLaser->Draw();
					Map();
					player->Draw();

					SetDrawBright(map.alpha[RED] - 50, map.alpha[GREEN] - 50, map.alpha[BLUE] - 50);
					DrawGraph(0, 0, vignette, true);
					SetDrawBright(255, 255, 255);

					if (isClear == 0) {
						ui->ButtonDraw(pad, padOld, player->transform);

						if (pad.Buttons[XINPUT_BUTTON_Y] == 1 && padOld.Buttons[XINPUT_BUTTON_Y] == 0) {
							GetDrawScreenGraph(0, 0, WIN_WIDTH, WIN_HEIGHT, gauss);
							DrawGraph(1265, WIN_HEIGHT - 48, ui->yButtanPic[1], true);
							}
						else if (pad.Buttons[XINPUT_BUTTON_Y] == 1 && padOld.Buttons[XINPUT_BUTTON_Y] == 1) {
							DrawGraph(1265, WIN_HEIGHT - 48, ui->yButtanPic[1], true);
							}
						else {
							DrawGraph(1265, WIN_HEIGHT - 48, ui->yButtanPic[0], true);
							}
						}

					//------クリア-----
					else if (isClear == 1) {

						if (clearAlpha < 140) {
							GetDrawScreenGraph(0, 0, WIN_WIDTH, WIN_HEIGHT, blurHandle);
							GraphFilter(blurHandle, DX_GRAPH_FILTER_GAUSS, 32, 1200);
							DrawGraph(0, 0, blurHandle, true);
							}
						//演出
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, clearAlpha);
						DrawRotaGraph2(map.crystalPos[map.stage][0], map.crystalPos[map.stage][1], 32, 32, ex, 0, light, true);
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

						//選択
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, clearAlpha2);
						DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, 1, 0.0, ui->pauseImage, true);
						DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, 0.8, 0.0, clearPic[clearPicNum], true);
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
						}

					if (tutorial->isGameTutorial == true) {
						tutorial->Draw();

						if (tutorial->Count == 40) {

							if (pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0 ||
								pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 1) {

								DrawRotaGraph(1180, 750, 1.4, 0.0, ui->bButtanPic[1], true);
								}
							else {
								DrawRotaGraph(1180, 750, 1.4, 0.0, ui->bButtanPic[0], true);
								}
							}
						}

					if (ui->isChange == true) {
						ui->DrawFadeInOut();
						}
					break;

				case 3://ポーズ画面-------------------------------------------------------------
					////後ろでゲーム画面を描画////
					DrawGraph(0, 0, map.bg[NONE], true);

					SetDrawBlendMode(DX_BLENDMODE_ADD, map.alpha[RED]);
					DrawGraph(0, 0, map.bg[RED], true);

					SetDrawBlendMode(DX_BLENDMODE_ADD, map.alpha[GREEN]);
					DrawGraph(0, 0, map.bg[GREEN], true);

					SetDrawBlendMode(DX_BLENDMODE_ADD, map.alpha[BLUE]);
					DrawGraph(0, 0, map.bg[BLUE], true);

					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
					redLaser->Draw();
					greenLaser->Draw();
					blueLaser->Draw();
					Map();
					player->Draw();
					SetDrawBright(map.alpha[RED], map.alpha[GREEN], map.alpha[BLUE]);
					DrawGraph(0, 0, vignette, true);
					SetDrawBright(255, 255, 255);

					GetDrawScreenGraph(0, 0, WIN_WIDTH, WIN_HEIGHT, blurHandle);
					GraphFilter(blurHandle, DX_GRAPH_FILTER_GAUSS, 32, 1200);
					DrawGraph(0, 0, blurHandle, true);
					/////-----------------////
					ui->drawColorPalette(1275, 40, colorPalette);
					ui->PauseDraw();

					if (pad.Buttons[XINPUT_BUTTON_Y] == 1 && padOld.Buttons[XINPUT_BUTTON_Y] == 0 ||
						pad.Buttons[XINPUT_BUTTON_Y] == 1 && padOld.Buttons[XINPUT_BUTTON_Y] == 1) {

						DrawGraph(1265, WIN_HEIGHT - 48, ui->yButtanPic[3], true);
						}
					else {
						DrawGraph(1265, WIN_HEIGHT - 48, ui->yButtanPic[2], true);
						}


					if (ui->isChange == true) {
						ui->DrawFadeInOut();
						}

					DrawFormatString(1000, 20, GetColor(255, 255, 255), "LX %d\nLY %d", pad.ThumbLX, pad.ThumbLY);
					DrawFormatString(1000, 60, GetColor(255, 255, 255), "RX %d\nRY %d", pad.ThumbRX, pad.ThumbRY);
					DrawFormatString(1000, 100, GetColor(255, 255, 255), "%lf", redLaser->theta);
					break;

				case 4:

					DrawFormatString(700, 480, GetColor(255, 255, 255), "GameOver\nBでリトライ");

					break;
			}
		//---------  ここまでにプログラムを記述  ---------//
		ScreenFlip();
		fps.Wait();        //待機

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

void BgColor() {
	int siHandle;
	int w, h;
	int r, g, b, a, gray;

	map.bg[1] = LoadGraph("Resources/background.png");
	siHandle = LoadSoftImage("Resources/background.png");
	//画像サイズ取得
	GetSoftImageSize(siHandle, &w, &h);

	//画像のグレースケール化
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			//ピクセル単位の色取得
			GetPixelSoftImage(siHandle, x, y, &r, &g, &b, &a);
			//色の平均値を求める
			gray = (r + g + b) / 3;
			//求めた平均値に置き換える
			DrawPixelSoftImage(siHandle, x, y, gray, gray, gray, a);
			}
		}
	//グラフハンドルにする
	map.bg[NONE] = CreateGraphFromSoftImage(siHandle);
	DeleteGraph(siHandle);

	//--------------rのみ-----------
	siHandle = LoadSoftImage("Resources/background.png");
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			//ピクセル単位の色取得
			GetPixelSoftImage(siHandle, x, y, &r, &g, &b, &a);
			//色の平均値を求める
			gray = (r + g + b) / 3;

			//平均値よりその色の値が大きければ置き換える
			if (r > gray) {
				DrawPixelSoftImage(siHandle, x, y, r - gray, 0, 0, a);
				}
			else {
				DrawPixelSoftImage(siHandle, x, y, 0, 0, 0, a);
				}
			}
		}
	map.bg[RED] = CreateGraphFromSoftImage(siHandle);
	DeleteGraph(siHandle);

	//-------------gのみ------------
	siHandle = LoadSoftImage("Resources/background.png");
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			//ピクセル単位の色取得
			GetPixelSoftImage(siHandle, x, y, &r, &g, &b, &a);
			//色の平均値を求める
			gray = (r + g + b) / 3;

			//平均値よりその色の値が大きければ置き換える
			if (g > gray) {
				DrawPixelSoftImage(siHandle, x, y, 0, g - gray, 0, a);
				}
			else {
				DrawPixelSoftImage(siHandle, x, y, 0, 0, 0, a);
				}
			}
		}
	map.bg[GREEN] = CreateGraphFromSoftImage(siHandle);
	DeleteGraph(siHandle);

	//------------bのみ------------
	siHandle = LoadSoftImage("Resources/background.png");
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			//ピクセル単位の色取得
			GetPixelSoftImage(siHandle, x, y, &r, &g, &b, &a);
			//色の平均値を求める
			gray = (r + g + b) / 3;

			//平均値よりその色の値が大きければ置き換える
			if (b > gray) {
				DrawPixelSoftImage(siHandle, x, y, 0, 0, b - gray, a);
				}
			else {
				DrawPixelSoftImage(siHandle, x, y, 0, 0, 0, a);
				}
			}
		}
	map.bg[BLUE] = CreateGraphFromSoftImage(siHandle);
	DeleteGraph(siHandle);
	}


double easeOutCirc(double x) {
	return pow(x, 4);
	}