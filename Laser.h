#pragma once
#include"DxLib.h"
#include"Map.h"
#include"Player.h"
#include "UI.h"


class Particle {
public:
	//------------�֐��錾---------------
	//������
	Particle();
	//����
	int Spawn(int posX, int posY);
	//�ړ�
	void Move(int posX, int posY, int isMove);
	//�`��
	void Draw();

	//------------�ϐ��錾----------------
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

	//�p�x�����֐�
	void Angle(XINPUT_STATE pad, XINPUT_STATE padOld, TransForm player);

	//Draw�֐�
	void Draw();

	//�ϐ��錾
	int max;
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


	Particle* particle[100];
	int spawnCount, spawnMax;
	int spawnTimer, spawnLimt;
	};