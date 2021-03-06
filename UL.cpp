#include "UI.h"
#include "DxLib.h"
#include "Sturct.h"
#include "Map.h"
#include"Colision.h"
#include "Animation.h"
#include "SE.h"

//コンストラクタの定義
UI::UI() {
	LoadDivGraph("Resources/b.png", 2, 2, 1, 32, 32, bButtanPic, true);
	LoadDivGraph("Resources/y.png", 4, 4, 1, 128, 32, yButtanPic, true);
	LoadDivGraph("Resources/Rstick.png", 8, 8, 1, 64, 64, rStickPic, true);
	LoadDivGraph("Resources/pauseFont.png", 4, 4, 1, 800, 510, pauseFontImage, true);
	pauseImage = LoadGraph("Resources/poss.png");
	ControlImage = LoadGraph("Resources/poss.png");
	xBoxImage = LoadGraph("Resources/XBox.png");
	colorFrameImage = LoadGraph("Resources/colorFrame.png");
	magnification = 0;
	buttan = 0;
	isContorolMethod = 0;
	ContorolMagnification = 0;
	isEndPause = 0;
	fade = 0;
	isIn = 0;
	isOut = 0;
	isChange = 0;
	returnTitle = 0;
	direction = 0;

	anime = new Animation;
	}

//シーン切り替えインアウト
void UI::SceneTransition() {
	//更新処理
	if (isChange == true) {
		if (isIn == true) {

			fade += 15;
			if (fade >= 235) {
				isOut = true;
				isIn = false;
				}
			}

		if (isOut == true) {
			fade -= 10;

			if (fade <= 0) {
				fade = 0;
				isOut = false;
				isIn = false;
				isChange = false;
				}
			}
		}
	}

void UI::DrawFadeInOut() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade);
	DrawBox(0, 0, WIN_WIDTH, WIN_HEIGHT, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

//ポーズ中の処理
void UI::PauseUpdate(XINPUT_STATE pad, XINPUT_STATE padOld, int& scene, int& reset, int& alpha, int& titleAlpha) {
	//ポーズのウィンドウを閉じる
	if (isEndPause == true) {
		magnification -= 0.2;

		if (magnification < 0.0) {
			magnification = 0.0;
			}
		}
	//ポーズ画面を表示中
	else {
		magnification += 0.2;

		if (magnification > 1.0) {
			magnification = 1.0;
			}
		}
	//選択
	if (pad.ThumbLY > 9000 && padOld.ThumbLY < 9000 && isChange == false) {
		if (buttan != 0) {
			buttan--;
			}
		}
	else if (pad.ThumbLY < -9000 && padOld.ThumbLY>-9000 && isChange == false) {
		if (buttan != 3) {
			buttan++;
			}
		}

	//操作方法画面でYを押したら
	if (pad.Buttons[XINPUT_BUTTON_Y] == 1 && padOld.Buttons[XINPUT_BUTTON_Y] == 0 && isContorolMethod == true) {
		isContorolMethod = false;
		}
	//ポーズ画面でYを押したら
	else if (pad.Buttons[XINPUT_BUTTON_Y] == 1 && padOld.Buttons[XINPUT_BUTTON_Y] == 0 && isContorolMethod == false) {
		isEndPause = true;	//ポーズ終了
		}

	//ポーズ閉じるアニメーションが終わったら
	if (magnification <= 0.0) {
		scene = 2;	//ゲーム画面に戻る
		isContorolMethod = false;
		isEndPause = false;
		}

	//決定
	if (pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0) {
		//操作方法確認
		if (buttan == 2) {
			isContorolMethod = true;	//操作方法画面を開く
			}
		if (isChange == false && isContorolMethod == false) {
			isChange = true;
			isIn = true;
			}
		}
	//タイトル画面へ
	if (buttan == 3) {
		if (fade >= 225) {
			returnTitle = true;
			scene = 0;
			alpha = 0;
			titleAlpha = 0;
			}
		}
	//ステージ画面選択へ
	else if (buttan == 1) {
		if (fade >= 225) {
			//StopSoundMem(titleBGM);
			scene = 1;
			}
		}
	//スタートからやり直す
	else if (buttan == 0) {

		if (fade >= 225) {
			reset = 1;
			scene = 2;
			}
		}

	//操作方法画面を開いている
	if (isContorolMethod == true && isEndPause == false) {
		ContorolMagnification += 0.2;

		if (ContorolMagnification > 1.4) {
			ContorolMagnification = 1.4;
			}
		}
	//操作方法画面を閉じる
	else if (isContorolMethod == false) {
		ContorolMagnification -= 0.2;

		if (ContorolMagnification < 0.0) {
			ContorolMagnification = 0.0;
			}
		}
	}

void UI::ButtonDraw(XINPUT_STATE pad, XINPUT_STATE padOld, TransForm player) {
	for (int y = 0; y < 30; y++) {
		for (int x = 0; x < 44; x++) {
			if (map.CHIP[map.stage][y][x] == R_LASER) {
				pointX[0] = x * 32 + 16;
				pointY[0] = y * 32 + 16;
			}

			if (map.CHIP[map.stage][y][x] == G_LASER) {
				pointX[1] = x * 32 + 16;
				pointY[1] = y * 32 + 16;
			}

			if (map.CHIP[map.stage][y][x] == B_LASER) {
				pointX[2] = x * 32 + 16;
				pointY[2] = y * 32 + 16;
			}
		}
	}
	//方向
	if (pad.ThumbLX > 9000 && isLaserMode == 0) {
		direction = RIGHT;
	}

	if (pad.ThumbLX < -9000 && isLaserMode == 0) {
		direction = LEFT;
	}

	if (pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0 ||
		pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 1) {
		isPush = 1;
		}
	else {
		isPush = 0;
		}

	anime->animation(anime->rStickFrame, anime->rStickTimer, 7, 8);
	//描画処理
	//方向によって変化

	for (int i = 0; i < 3; i++) {

		if (Colision(player, pointX[i], pointY[i])) {
			if (pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0) {
				if (isLaserMode == 0) {
					isLaserMode = 1;
				}
				else {
					isLaserMode = 0;
				}
			}
		}

		if (direction == RIGHT) {
			//Bボタン押してる時
			if (isPush == 1 && Colision(player, pointX[i], pointY[i])) {

				DrawGraph(
					player.posX + player.r,
					player.posY - player.r - 16,
					bButtanPic[1], true);
			}
			//Bボタン押してない時
			else if (Colision(player, pointX[i], pointY[i])) {
				DrawGraph(
					player.posX + player.r,
					player.posY - player.r - 16,
					bButtanPic[0], true);

				if (isLaserMode == true) {
					DrawGraph(
						player.posX - player.r * 3,
						player.posY - player.r - 32,
						rStickPic[anime->rStickTimer], true);
				}
			}
		}
		else if (direction == LEFT) {
			//Bボタン押してる時
			if (isPush == 1 && Colision(player, pointX[i], pointY[i])) {

				DrawGraph(
					player.posX - player.r * 2,
					player.posY - player.r - 16,
					bButtanPic[1], true);
				}
			//Bボタン押してない時
			else if (Colision(player, pointX[i], pointY[i])) {
				DrawGraph(
					player.posX - player.r * 2,
					player.posY - player.r - 16,
					bButtanPic[0], true);

				if (isLaserMode == true) {
					DrawGraph(
						player.posX + player.r,
						player.posY - player.r - 32,
						rStickPic[anime->rStickTimer], true);
					}
				}
			}
		}
	}

//ポーズ中の描画関数
void UI::PauseDraw() {
	////----ポーズ画面描画----////
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220);
	DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, magnification, 0.0, pauseImage, true);

	if (buttan == 0) {
		DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, magnification - 0.3, 0.0, pauseFontImage[0], true);
		}
	else if (buttan == 1) {
		DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, magnification - 0.3, 0.0, pauseFontImage[1], true);
		}
	else if (buttan == 2) {
		DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, magnification - 0.3, 0.0, pauseFontImage[2], true);
		}
	else if (buttan == 3) {
		DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, magnification - 0.3, 0.0, pauseFontImage[3], true);
		}

	////----操作方法の画面描画----////
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, ContorolMagnification, 0.0, ControlImage, true);
	DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, ContorolMagnification, 0.0, xBoxImage, true);

	//デバッグ用
	}

void UI::drawColorPalette(int x, int y, int* graph) {
	DrawRotaGraph(x + 48, y + 48, 0.4, 0.0, colorFrameImage, true);

	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	if (map.isDraw[RED] == false && map.isDraw[GREEN] == false && map.isDraw[BLUE] == false) {
		SetDrawBright(100, 0, 0);
		DrawGraph(x, y, graph[0], true);
		SetDrawBright(0, 100, 0);
		DrawGraph(x + 32, y, graph[1], true);
		SetDrawBright(0, 0, 100);
		DrawGraph(x + 16, y + 32, graph[2], true);
		}
	else if (map.isDraw[RED] == true && map.isDraw[GREEN] == false && map.isDraw[BLUE] == false) {
		SetDrawBright(255, 0, 0);
		DrawGraph(x, y, graph[0], true);
		SetDrawBright(0, 100, 0);
		DrawGraph(x + 32, y, graph[1], true);
		SetDrawBright(0, 0, 100);
		DrawGraph(x + 16, y + 32, graph[2], true);
		}
	else if (map.isDraw[RED] == false && map.isDraw[GREEN] == true && map.isDraw[BLUE] == false) {
		SetDrawBright(100, 0, 0);
		DrawGraph(x, y, graph[0], true);
		SetDrawBright(0, 255, 0);
		DrawGraph(x + 32, y, graph[1], true);
		SetDrawBright(0, 0, 100);
		DrawGraph(x + 16, y + 32, graph[2], true);
		}
	else if (map.isDraw[RED] == false && map.isDraw[GREEN] == false && map.isDraw[BLUE] == true) {
		SetDrawBright(100, 0, 0);
		DrawGraph(x, y, graph[0], true);
		SetDrawBright(0, 100, 0);
		DrawGraph(x + 32, y, graph[1], true);
		SetDrawBright(0, 0, 255);
		DrawGraph(x + 16, y + 32, graph[2], true);
		}
	else if (map.isDraw[RED] == true && map.isDraw[GREEN] == true && map.isDraw[BLUE] == false) {
		SetDrawBright(255, 0, 0);
		DrawGraph(x, y, graph[0], true);
		SetDrawBright(0, 255, 0);
		DrawGraph(x + 32, y, graph[1], true);
		SetDrawBright(0, 0, 100);
		DrawGraph(x + 16, y + 32, graph[2], true);
		}
	else if (map.isDraw[RED] == true && map.isDraw[GREEN] == true && map.isDraw[BLUE] == true) {
		SetDrawBright(255, 0, 0);
		DrawGraph(x, y, graph[0], true);
		SetDrawBright(0, 255, 0);
		DrawGraph(x + 32, y, graph[1], true);
		SetDrawBright(0, 0, 255);
		DrawGraph(x + 16, y + 32, graph[2], true);
		}
	else if (map.isDraw[RED] == true && map.isDraw[GREEN] == false && map.isDraw[BLUE] == true) {
		SetDrawBright(255, 0, 0);
		DrawGraph(x, y, graph[0], true);
		SetDrawBright(0, 100, 0);
		DrawGraph(x + 32, y, graph[1], true);
		SetDrawBright(0, 0, 255);
		DrawGraph(x + 16, y + 32, graph[2], true);
		}
	else if (map.isDraw[RED] == false && map.isDraw[GREEN] == true && map.isDraw[BLUE] == true) {
		SetDrawBright(100, 0, 0);
		DrawGraph(x, y, graph[0], true);
		SetDrawBright(0, 255, 0);
		DrawGraph(x + 32, y, graph[1], true);
		SetDrawBright(0, 0, 255);
		DrawGraph(x + 16, y + 32, graph[2], true);
		}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	SetDrawBright(255, 255, 255);
	}