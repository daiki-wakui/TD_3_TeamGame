#include"Laser.h"
#include<math.h>
#include"DxLib.h"
#include"Colision.h"

float easeInQuint(float x) {
	return pow(x, 3);
	}


//初期化
Particle::Particle() {
	startX = 0;
	startY = 0;
	endX = 0;
	endY = 0;
	isDraw = 0;
	lengthX = 0;
	lengthStartY = 0;
	lengthEndY = 0;
	timer = 0;
	limit = 25;
	timePerLimt = 0;
	color[0] = GetColor(235, 246, 247);
	color[1] = GetColor(161, 216, 226);
	color[2] = GetColor(0, 149, 217);
	color[3] = GetColor(0, 153, 204);
	color[4] = GetColor(132, 185, 203);
	colorRand = 0;
	}

//Spawn
int Particle::Spawn(int posX, int posY) {
	if (isDraw == 0) {
		isDraw = 1;
		//座標を引数を元に乱数で決める
		startX = posX + (rand() % 25 - 12);
		startY = posY - (rand() % 400);
		endX = startX;
		endY = startY - (rand() % 41 + 8);
		//消えるまでの時間
		limit = rand() % 50 + 25;
		//5色のうちどの色にするか乱数
		colorRand = rand() % 5;

		//引数との距離
		lengthX = startX - posX;
		lengthStartY = startY - posY;
		lengthEndY = endY - posY;

		//生成された数を把握するため変え値1
		return 1;
		}
	//生成されなかった
	return 0;
	}

//Move
void Particle::Move(int posX, int posY, int isMove) {
	//時間経過消滅
	timer++;
	timePerLimt = timer / limit;
	if (timer > limit) {
		isDraw = 0;
		timer = 0;
		limit = 0;
		}


	if (isMove == 0) {
		//引数との距離
		lengthX = startX - posX;
		lengthStartY = startY - posY;
		lengthEndY = endY - posY;
		//流れていく
		startY -= (10 * easeInQuint(timePerLimt));
		endY -= (10 * easeInQuint(timePerLimt));
		}
	//移動
	else if (isMove == 1) {
		//引数との距離を保ちつつ移動
		startX = posX + lengthX;
		endX = posX + lengthX;
		lengthStartY -= (10 * easeInQuint(timePerLimt));
		lengthEndY -= (10 * easeInQuint(timePerLimt));
		startY = posY + lengthStartY;
		endY = posY + lengthEndY;
		}
	}

//Draw
void Particle::Draw() {
	if (isDraw == 1) {
		DrawLineAA(startX, startY, endX, endY, color[colorRand]);
		}
	}

//--------------------------------------------------------------------------------------------

Laser::Laser(MAP map, int blockColor) {
	LoadDivGraph("Laser.png", 3, 3, 1, 96, 64, pic, true);

	for (int y = 0; y < 30; y++) {
		for (int x = 0; x < 44; x++) {
			if (map.CHIP[map.stage][y][x] == blockColor) {
				startX[0] = x * 32 + 16;
				startY[0] = y * 32 + 16;
				}
			}
		}

	color = blockColor;
	if (color == R_LASER) {
		colorCode = GetColor(200, 0, 0);
		}
	else if (color == G_LASER) {
		colorCode = GetColor(0, 200, 0);
		}
	else if (color == B_LASER) {
		colorCode = GetColor(0, 0, 200);
		}

	endX[0] = startX[0];
	endY[0] = 0;
	theta = 1.5;
	thetaOld = 0;
	angle[0] = 1.56;
	isAngle = 0;
	for (int i = 1; i < 10; i++) {
		isDraw[i] = 0;
		angle[i] = 0;
		isHitX[i] = 0;
		isHitY[i] = 0;
		isOldHitX[i] = 0;
		isOldHitY[i] = 0;
		}
	isDraw[0] = 1;
	max = 10;
	}


void Laser::Angle(XINPUT_STATE pad, XINPUT_STATE padOld, TransForm player) {//開始
	if (isAngle == 0) {
		if (Colision(player, startX[0], startY[0])) {
			if (pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0) {
				isAngle = 1;
				}
			}
		}
	//角度変更
	else if (isAngle == 1) {
		thetaOld = theta;
		//スティック角度取得
		if (pad.ThumbRX > 16000 || pad.ThumbRX < -16000 || pad.ThumbRY > 16000 || pad.ThumbRY < -16000) {
			theta = atan2(pad.ThumbRY, pad.ThumbRX);
			}

		//左回転
		if (theta > thetaOld || (theta < -2.7 && thetaOld > 2.7)) {
			if (theta > 2.7 && thetaOld < -2.7) {
				}
			else if (theta - thetaOld > 0.3) {
				angle[0] += 0.04;
				}
			else if (theta - thetaOld > 0.1) {
				angle[0] += 0.02;
				}
			else {
				angle[0] += 0.005;
				}
			}
		//右回転
		else if (theta < thetaOld || (theta > 2.7 && thetaOld < -2.7)) {
			if (thetaOld - theta > 0.3) {
				angle[0] -= 0.04;
				}
			else if (thetaOld - theta > 0.1) {
				angle[0] -= 0.02;
				}
			else if (thetaOld - theta > 0) {
				angle[0] -= 0.005;
				}
			}

		//終了
		if (Colision(player, startX[0], startY[0])) {
			if (pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0) {
				isAngle = 0;
				}
			}
		}

	for (int i = 0; i < max; i++) {

		if (isDraw[i] == 1) {
			//レーザー延長
			endX[i] = startX[i] + (cos(angle[i]) * 2000);
			endY[i] = startY[i] + (sin(angle[i]) * -2000);

			for (int y = 0; y < 30; y++) {
				for (int x = 0; x < 44; x++) {

					//一番近い壁で止まる
					if (map.isDraw[map.CHIP[map.stage][y][x]] == 1) {
						if (LineBox(startX[i], startY[i], &endX[i], &endY[i], map, x, y)) {
							if (i < (max - 1)) {
								for (int j = i + 1; j < max; j++) {
									isDraw[j] = 0;
									}
								}
							}
						}

					//レーザーと鏡
					if (LineMirror(startX[i], startY[i], &endX[i], &endY[i], map, x, y) == 1) {
						if (i < (max - 1)) {
							isDraw[i + 1] = 1;
							if (map.CHIP[map.stage][y][x] == 11) {
								//上
								if (LineLine(startX[i], startY[i], &endX[i], &endY[i], 32 * x, 32 * y, 32 * x + 32, 32 * y)) {
									startX[i + 1] = endX[i];
									startY[i + 1] = endY[i] - 1;
									angle[i + 1] = atan2(0, 32) + (atan2(0, 32) - angle[i]);
									}
								//右
								if (LineLine(startX[i], startY[i], &endX[i], &endY[i], 32 * x + 32, 32 * y, 32 * x + 32, 32 * y + 32)) {
									startX[i + 1] = endX[i] + 1;
									startY[i + 1] = endY[i];
									angle[i + 1] = atan2(32, 0) + (atan2(32, 0) - angle[i]);
									}
								//下
								if (LineLine(startX[i], startY[i], &endX[i], &endY[i], 32 * x, 32 * y + 32, 32 * x + 32, 32 * y + 32)) {
									startX[i + 1] = endX[i];
									startY[i + 1] = endY[i] + 1;
									angle[i + 1] = atan2(0, 32) + (atan2(0, 32) - angle[i]);
									}
								//左
								if (LineLine(startX[i], startY[i], &endX[i], &endY[i], 32 * x, 32 * y, 32 * x, 32 * y + 32)) {
									startX[i + 1] = endX[i] - 1;
									startY[i + 1] = endY[i];
									angle[i + 1] = atan2(32, 0) + (atan2(32, 0) - angle[i]);
									}
								}
							if (map.CHIP[map.stage][y][x] == 12) {
								startX[i + 1] = endX[i] + 1;
								startY[i + 1] = endY[i] + 1;
								angle[i + 1] = atan2(32, 32) + (atan2(32, 32) - angle[i]);
								}
							else if (map.CHIP[map.stage][y][x] == 13) {
								startX[i + 1] = endX[i] - 1;
								startY[i + 1] = endY[i] + 1;
								angle[i + 1] = atan2(-32, 32) + (atan2(-32, 32) - angle[i]);
								}
							else if (map.CHIP[map.stage][y][x] == 14) {
								startX[i + 1] = endX[i] - 1;
								startY[i + 1] = endY[i] - 1;
								angle[i + 1] = atan2(32, 32) + (atan2(32, 32) - angle[i]);
								}
							else if (map.CHIP[map.stage][y][x] == 15) {
								startX[i + 1] = endX[i] + 1;
								startY[i + 1] = endY[i] - 1;
								angle[i + 1] = atan2(-32, 32) + (atan2(-32, 32) - angle[i]);
								}
							}
						}
					}
				}

			if (endX[i] < 0 || endX[i]>1408 || endY[i] < 0 || endY[i]>960) {
				if (i < (max - 1)) {
					isDraw[i + 1] = 0;
					}
				}
			}
		else if (isDraw[i] == 0) {
			if (i < (max - 1)) {
				for (int j = i + 1; j < 10; j++) {
					isDraw[j] = 0;
					}
				}
			}

		if (isDraw[i] == 1) {
			//クリスタルとレーザー
			if (LineCrystal(startX[i], startY[i], &endX[i], &endY[i],
				map.crystalPos[map.stage][1] - 86, map.crystalPos[map.stage][1] + 86, map.crystalPos[map.stage][1] + 32,
				map.crystalPos[map.stage][0] - 41, map.crystalPos[map.stage][0] + 38, map.crystalPos[map.stage][0])) {
				map.isDraw[color - 6] = true;
				if (i < (max - 1)) {
					for (int j = i + 1; j < max; j++) {
						isDraw[j] = 0;
						}
					}
				}
			else {
				map.isDraw[color - 6] = false;
				}
			}
		}
	}

void Laser::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	for (int i = 0; i < max; i++) {
		if (isDraw[i] == 1) {
			DrawLineAA(startX[i], startY[i], endX[i], endY[i], colorCode, 16);
			}
		}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawRotaGraph2(startX[0], startY[0], 17, 32, 1, -angle[0], pic[color - 8], true, false);
	}