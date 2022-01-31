#pragma once
#include "DxLib.h"
#include "Sturct.h"
#include "Animation.h"

class UI {
public:

	//画像
	int bButtanPic[2];
	int rStickPic[8];
	//ポーズで使う変数
	int pauseImage;
	int ControlImage;
	int xBoxImage;
	float magnification;
	int buttan;
	int isContorolMethod;
	float ContorolMagnification;
	int isEndPause;
	//シーン切り替えで使う変数
	int fade;
	int isIn;
	int isOut;
	int isChange;
	int returnTitle;
	//ステージのUI
	int pointX[3];
	int pointY[3];
	int direction;
	int isPush;
	int isLaserMode;

	enum PlayerDirection {
		RIGHT,
		LEFT
	};

	Animation* anime;
public:
	UI();

	void SceneTransition();
	void DrawFadeInOut();
	void PauseDraw();
	void PauseUpdate(XINPUT_STATE pad, XINPUT_STATE padOld, int& scene,int &reset);
	void ButtonDraw(XINPUT_STATE pad, XINPUT_STATE padOld,TransForm player);
	void drawColorPalette(int x, int y, int* graph);
};


