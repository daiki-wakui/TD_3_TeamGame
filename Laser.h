#pragma once
#include"DxLib.h"
#include"Map.h"
#include"Player.h"
#include "UI.h"


class Particle {
public:
	//------------関数宣言---------------
	//初期化
	Particle();
	//発生
	int Spawn(int posX, int posY);
	//移動
	void Move(int posX, int posY, int isMove);
	//描画
	void Draw();

	//------------変数宣言----------------
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

	//角度調整関数
	void Angle(XINPUT_STATE pad, XINPUT_STATE padOld, TransForm player);

	//Draw関数
	void Draw();

	//変数宣言
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