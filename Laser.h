#pragma once
#include"DxLib.h"
#include"Map.h"
#include"Player.h"
#include "UI.h"


class Particle {
public:
	//------------ŠÖ”éŒ¾---------------
	//‰Šú‰»
	Particle();
	//”­¶
	int Spawn(int posX, int posY);
	//ˆÚ“®
	void Move(int posX, int posY, int isMove);
	//•`‰æ
	void Draw();

	//------------•Ï”éŒ¾----------------
	int isDraw;
	float startX, startY, endX, endY;
	float lengthX, lengthStartY, lengthEndY;
	float timer, limit;
	float timePerLimt;
	int color[5];
	int colorRand;
	};

class Laser {
public:
	Laser(MAP map, int blockColor);
	void Reset();

	//Šp“x’²®ŠÖ”
	void Angle(XINPUT_STATE pad, XINPUT_STATE padOld, TransForm player);

	//DrawŠÖ”
	void Draw();

	//•Ï”éŒ¾
	int max;
	int pic[3];
	int isDraw[20];
	float startX[20], startY[20];
	float endX[20], endY[20];
	int color, colorCode;
	double theta, thetaOld;
	int isAngle;
	double angle[20];



	Particle* particle[100];
	int spawnCount, spawnMax;
	int spawnTimer, spawnLimt;
	};