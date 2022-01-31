#pragma once
#include"Sturct.h"
#include "DxLib.h"

enum BLOCK {
	NONE,		//0
	NORMAL,		//1
	RED,		//2
	GREEN,		//3
	BLUE,		//4
	CYAN,		//5
	MAGENTA,	//6
	YELLOW,		//7
	R_LASER,	//8
	G_LASER,	//9
	B_LASER,	//10
	};

typedef struct MAP {
	int stage;			//���̃X�e�[�W
	int bg[5];			//�w�i�摜
	int chipBlock[16];	//���\�[�X�f�[�^�����ϐ�
	int isDraw[16];		//�`��t���O
	int alpha[8];		//�A���t�@
	const int CHIP[5][30][44];	//�X�e�[�W�̃}�b�v�`�b�v���
	int crystalPic;
	int crystalPos[5][2];//�X�e�[�W���̃N���X�^�����W
	int crystalSize[3];//�N���X�^���̑傫��
};
extern MAP map;

void Map();	//�`��֐�


void drawColorPalette(int x, int y, int* graph);