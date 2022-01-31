#pragma once
#include"Sturct.h"
#include"DxLib.h"
#include "Animation.h"
#include "UI.h"

class Player {
public:
	//������
	Player();
	void Reset();
	//Move�֐�
	void Move(XINPUT_STATE pad, XINPUT_STATE padOld);
	void Move(char* keys, char* oldkeys);
	//Draw�֐�
	void Draw();
	//�}�b�v�`�b�v���W�̏����֐�
	void MapchipInfo();
	void MapchipTemporaryInfo();

	//�\����(posX,posY,r)
	TransForm transform;

	Animation* anime;

	////////////////////�ϐ��錾////////////////////
	int idleTimer;
	//�摜
	int Pic;
	int idlePic[7];

	int glidePic[2];
	//���
	int state;
	//����
	int direction;
	int hitSide;
	//�ړ� 
	int walkSpeed;
	int dashSpeed;
	int speedTmp;
	//�W�����v
	int isJump;
	int jumpSpeed;
	int jumpTimer;
	//�O���C�_�[
	int isGliding;
	int glidingG;
	int isGliderColliding;
	int hasGlided;
	//�d��
	int g;
	//���[�U�[
	int isMove;
	//�}�b�v�`�b�v���W
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