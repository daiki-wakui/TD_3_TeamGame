#pragma once
#include"DxLib.h"
#include"Map.h"
#include"Player.h"
#include "UI.h"

class Laser {
public:
	Laser(MAP map, int blockColor);
	void Reset();

	//Šp“x’²®ŠÖ”
	void Angle(XINPUT_STATE pad, XINPUT_STATE padOld, TransForm player);

	//DrawŠÖ”
	void Draw();

	//•Ï”éŒ¾
	int pic[3];
	int isDraw[10];
	float startX[10], startY[10];
	float endX[10], endY[10];
	int color, colorCode;
	double theta, thetaOld;
	int isAngle;
	double angle[10];

	int isHitX[10], isHitY[10];
	int isOldHitX[10], isOldHitY[10];
};
