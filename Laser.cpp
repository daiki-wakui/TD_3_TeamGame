#include"Laser.h"
#include<math.h>
#include"DxLib.h"
#include"Colision.h"
#include "UI.h"

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
}


void Laser::Angle(XINPUT_STATE pad, XINPUT_STATE padOld, TransForm player) {//�J�n
	if (isAngle == 0) {
		if (Colision(player, startX[0], startY[0])) {
			if (pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0) {
				isAngle = 1;
			}
		}
	}
	//�p�x�ύX
	else if (isAngle == 1) {
		thetaOld = theta;
		//�X�e�B�b�N�p�x�擾
		if (pad.ThumbRX > 16000 || pad.ThumbRX < -16000 || pad.ThumbRY > 16000 || pad.ThumbRY < -16000) {
			theta = atan2(pad.ThumbRY, pad.ThumbRX);
			}

		//����]
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
		//�E��]
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

		//�I��
		if (Colision(player, startX[0], startY[0])) {
			if (pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0) {
				isAngle = 0;
				}
			}
		}

	for (int i = 0; i < 10; i++) {

		if (isDraw[i] == 1) {
			//���[�U�[����
			endX[i] = startX[i] + (cos(angle[i]) * 2000);
			endY[i] = startY[i] + (sin(angle[i]) * -2000);

			for (int y = 0; y < 30; y++) {
				for (int x = 0; x < 44; x++) {
					//��ԋ߂��ǂŎ~�܂�
					/*if (map.CHIP[map.stage][y][x] == 1) {
						if (LineBox(startX[i], startY[i], &endX[i], &endY[i], y * 32, y * 32 + 32, x * 32, x * 32 + 32)) {
							if (i < 9) {
								for (int j = i + 1; j < 10; j++) {
									isDraw[j] = 0;
									}
								}
							}
						}*/

						//��ԋ߂��ǂŎ~�܂�
					if (map.isDraw[map.CHIP[map.stage][y][x]] == 1) {
						if (LineBox(startX[i], startY[i], &endX[i], &endY[i], y * 32, y * 32 + 32, x * 32, x * 32 + 32)) {
							if (i < 9) {
								for (int j = i + 1; j < 10; j++) {
									isDraw[j] = 0;
									}
								}
							}
						}

					//���[�U�[�Ƌ�
					if (LineMirror(startX[i], startY[i], &endX[i], &endY[i], map, x, y) == 1) {
						if (i < 9) {
							isDraw[i + 1] = 1;
							if (map.CHIP[map.stage][y][x] == 11) {
								//��
								if (LineLine(startX[i], startY[i], &endX[i], &endY[i], 32 * x, 32 * y, 32 * x + 32, 32 * y)) {
									startX[i + 1] = endX[i];
									startY[i + 1] = endY[i] - 1;
									angle[i + 1] = atan2(0, 32) + (atan2(0, 32) - angle[i]);
								}
								//�E
								if (LineLine(startX[i], startY[i], &endX[i], &endY[i], 32 * x + 32, 32 * y, 32 * x + 32, 32 * y + 32)) {
									startX[i + 1] = endX[i] + 1;
									startY[i + 1] = endY[i];
									angle[i + 1] = atan2(32, 0) + (atan2(32, 0) - angle[i]);
								}
								//��
								if (LineLine(startX[i], startY[i], &endX[i], &endY[i], 32 * x, 32 * y + 32, 32 * x + 32, 32 * y + 32)) {
									startX[i + 1] = endX[i];
									startY[i + 1] = endY[i] + 1;
									angle[i + 1] = atan2(0, 32) + (atan2(0, 32) - angle[i]);
								}
								//��
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
				if (i < 9) {
					isDraw[i + 1] = 0;
					}
				}
			}
		else if (isDraw[i] == 0) {
			if (i < 9) {
				for (int j = i + 1; j < 10; j++) {
					isDraw[j] = 0;
					}
				}
			}

		if (isDraw[i] == 1) {
			//�N���X�^���ƃ��[�U�[
			if (LineCrystal(startX[i], startY[i], &endX[i], &endY[i],
				map.crystalPos[map.stage][1] - 86, map.crystalPos[map.stage][1] + 86, map.crystalPos[map.stage][1] + 32,
				map.crystalPos[map.stage][0] - 41, map.crystalPos[map.stage][0] + 38, map.crystalPos[map.stage][0])) {
				map.isDraw[color - 6] = true;
				if (i < 9) {
					for (int j = i + 1; j < 10; j++) {
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
	for (int i = 0; i < 10; i++) {
		if (isDraw[i] == 1) {
			DrawLineAA(startX[i], startY[i], endX[i], endY[i], colorCode, 16);
			}
		}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawRotaGraph2(startX[0], startY[0], 17, 32, 1, -angle[0], pic[color - 8], true, false);
}