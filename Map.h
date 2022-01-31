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
	int stage;			//今のステージ
	int bg[5];			//背景画像
	int chipBlock[16];	//リソースデータ入れる変数
	int isDraw[16];		//描画フラグ
	int alpha[8];		//アルファ
	const int CHIP[5][30][44];	//ステージのマップチップ情報
	int crystalPic;
	int crystalPos[5][2];//ステージ毎のクリスタル座標
	int crystalSize[3];//クリスタルの大きさ
};
extern MAP map;

void Map();	//描画関数


void drawColorPalette(int x, int y, int* graph);