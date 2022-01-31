#pragma once
#include"Sturct.h"
#include"DxLib.h"
#include "Animation.h"
#include "UI.h"

class Player {
public:
	//初期化
	Player();
	void Reset();
	//Move関数
	void Move(XINPUT_STATE pad, XINPUT_STATE padOld);
	void Move(char* keys, char* oldkeys);
	//Draw関数
	void Draw();
	//マップチップ座標の所得関数
	void MapchipInfo();
	void MapchipTemporaryInfo();

	//構造体(posX,posY,r)
	TransForm transform;

	Animation* anime;

	////////////////////変数宣言////////////////////
	int idleTimer;
	//画像
	int Pic;
	int idlePic[7];

	int glidePic[2];
	//状態
	int state;
	//方向
	int direction;
	int hitSide;
	//移動 
	int walkSpeed;
	int dashSpeed;
	int speedTmp;
	//ジャンプ
	int isJump;
	int jumpSpeed;
	int jumpTimer;
	//グライダー
	int isGliding;
	int glidingG;
	int isGliderColliding;
	int hasGlided;
	//重力
	int g;
	//レーザー
	int isMove;
	//マップチップ座標
	int leftTopX;
	int leftTopY;
	int leftUnderX;
	int leftUnderY;
	int rightTopX;
	int rightTopY;
	int rightUnderX;
	int rightUnderY;
	int rightCentralX;
	int rightCentralY;
	int leftCentralX;
	int leftCentralY;
	int CentralY;
	int CentralX;

	enum Direction {
		RIGHT,
		LEFT
	};

	enum STATE {
		IDLE,
		WALK,
		DASH
	};
};