#include "Tutorial.h"
#include "DxLib.h"
#include "Map.h"

Tutorial::Tutorial() {
	Count = 0;
	if (map.stage == 0) {
		Page = 0;
		}
	else if (map.stage == 1) {
		Page = 5;
		}
	isGameTutorial = 1;
	framePic = LoadGraph("Resources/poss.png");
	LoadDivGraph("Resources/tutorial.png", 6, 6, 1, 1408, 960, fontPic, true);
	}

void Tutorial::Update(XINPUT_STATE pad, XINPUT_STATE padOld, int& scene) {
	if (scene == 1) {
		//1ステージと2ステージ目でチュートリアル
		if (map.stage == 0 || map.stage == 1) {
			isGameTutorial = true;
			Count = 0;
			}

		if (map.stage == 0) {
			Page = 0;
			}
		else if (map.stage == 1) {
			Page = 5;
			}
		}

	if (scene == 2) {

		if (isGameTutorial == true) {
			Count++;

			if (Count >= 40) {
				Count = 40;
				}

			if (map.stage == 0 && Count == 40 &&
				pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0) {

				if (Page == 3 || Page == 4) {
					isGameTutorial = false;
					}

				Page++;
				}

			if (map.stage == 1 && Count == 40 &&
				pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0) {

				if (Page == 5) {
					isGameTutorial = false;
					}
				}
			}
		}
	}

void Tutorial::Draw() {
	if (isGameTutorial == true && Count == 40) {
		DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, 1.5, 0.0, framePic, true);
		if (Page == 0) {
			DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, 0.6, 0.0, fontPic[0], true);
			}
		if (Page == 1) {
			DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, 0.6, 0.0, fontPic[1], true);
			}
		if (Page == 2) {
			DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, 0.6, 0.0, fontPic[2], true);
			}
		if (Page == 3) {
			DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, 0.6, 0.0, fontPic[3], true);
			}
		if (Page == 4) {
			DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, 0.6, 0.0, fontPic[4], true);
			}
		if (Page == 5) {
			DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, 0.6, 0.0, fontPic[5], true);
			}
		}
	/*DrawFormatString(500, 530, GetColor(255, 255, 255), "チュートリアル　%d/5", Page);
	DrawFormatString(500, 550, GetColor(255, 255, 255), "カウント　%d", Count);
	DrawFormatString(500, 570, GetColor(255, 255, 255), "チュートリアルフラグ　%d", isGameTutorial);*/
	}
