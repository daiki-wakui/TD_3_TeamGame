#include "Colision.h"

int Colision(TransForm transform, MAP map, int x, int y, int hitSide) {
	if (transform.posX + hitSide > 32 * x && transform.posX - hitSide < (32 * x) + 32) {
		if (transform.posY + transform.r > 32 * y && transform.posY - transform.r < (32 * y) + 32) {
			return map.CHIP[map.stage][y][x];
			}
		}
	}

//���[�U�[���u�ƃv���C���[
int Colision(TransForm transform, int x, int y) {
	if (transform.posX + transform.r > x - 16 && transform.posX - transform.r < x + 16 &&
		transform.posY + transform.r > y - 16 && transform.posY - transform.r < y + 16) {
		return 1;
		}
	else {
		return 0;
		}
	}

//���[�U�[�ƕӂ̌�_
bool LineLine(float laserStartX, float laserStartY, float* laserEndX, float* laserEndY, float startX, float startY, float endX, float endY) {
	// �����_�̋������v�Z
	float uA = ((endX - startX) * (laserStartY - startY) - (endY - startY) * (laserStartX - startX)) / ((endY - startY) * (*laserEndX - laserStartX) - (endX - startX) * (*laserEndY - laserStartY));
	float uB = ((*laserEndX - laserStartX) * (laserStartY - startY) - (*laserEndY - laserStartY) * (laserStartX - startX)) / ((endY - startY) * (*laserEndX - laserStartX) - (endX - startX) * (*laserEndY - laserStartY));

	// ����uA����uB��0-1�̊ԏꍇ����������
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
		// ��_�̒l
		*laserEndX = laserStartX + (uA * (*laserEndX - laserStartX));
		*laserEndY = laserStartY + (uA * (*laserEndY - laserStartY));
		return 1;
		}
	return 0;
	}

//���[�U�[�ƃ}�b�v�`�b�v
bool LineBox(float laserStartX, float laserStartY, float* laserEndX, float* laserEndY, MAP map, int x, int y) {
	float top = y * 32, bottom = y * 32 + 32;
	float left = x * 32, right = x * 32 + 32;
	int isHit = 0;
	//��
	if (LineLine(laserStartX, laserStartY, laserEndX, laserEndY, left + 8, top + 8, right - 8, top + 8)) {
		isHit = 1;
		}
	//��
	if (LineLine(laserStartX, laserStartY, laserEndX, laserEndY, left + 8, bottom - 8, right - 8, bottom - 8)) {
		isHit = 1;
		}
	//��
	if (LineLine(laserStartX, laserStartY, laserEndX, laserEndY, left + 8, top + 8, left + 8, bottom - 8)) {
		isHit = 1;
		}
	//�E
	if (LineLine(laserStartX, laserStartY, laserEndX, laserEndY, right - 8, top + 8, right - 8, bottom - 8)) {
		isHit = 1;
		}

	//��
	if (map.isDraw[map.CHIP[map.stage][y][x + 1]] == 1) {
		if (LineLine(laserStartX, laserStartY, laserEndX, laserEndY, right - 8, top + 16, right + 8, top + 16)) {
			isHit = 1;
			}
		}
	//�c
	if (map.isDraw[map.CHIP[map.stage][y + 1][x]] == 1) {
		if (LineLine(laserStartX, laserStartY, laserEndX, laserEndY, left + 16, bottom - 8, left + 16, bottom + 8)) {
			isHit = 1;
			}
		}
	//�E��
	if (map.isDraw[map.CHIP[map.stage][y][x + 1]] == 0 && map.isDraw[map.CHIP[map.stage][y - 1][x]] == 0) {
		if (LineLine(laserStartX, laserStartY, laserEndX, laserEndY, right, top, right - 8, top + 8)) {
			isHit = 1;
			}
		}
	//�E��
	if (map.isDraw[map.CHIP[map.stage][y][x + 1]] == 0 && map.isDraw[map.CHIP[map.stage][y + 1][x]] == 0) {
		if (LineLine(laserStartX, laserStartY, laserEndX, laserEndY, right, bottom, right - 8, bottom - 8)) {
			isHit = 1;
			}
		}
	//����
	if (map.isDraw[map.CHIP[map.stage][y][x - 1]] == 0 && map.isDraw[map.CHIP[map.stage][y + 1][x]] == 0) {
		if (LineLine(laserStartX, laserStartY, laserEndX, laserEndY, left, bottom, left + 8, bottom - 8)) {
			isHit = 1;
			}
		}
	//����
	if (map.isDraw[map.CHIP[map.stage][y][x - 1]] == 0 && map.isDraw[map.CHIP[map.stage][y - 1][x]] == 0) {
		if (LineLine(laserStartX, laserStartY, laserEndX, laserEndY, left, top, left + 8, top + 8)) {
			isHit = 1;
			}
		}

	return isHit;
	}

//���[�U�[�ƃN���X�^��
bool LineCrystal(float laserStartX, float laserStartY, float* laserEndX, float* laserEndY,
	float top, float bottom, float middleY, float left, float right, float middleX) {
	int isHit = 0;
	//�c
	if (LineLine(laserStartX, laserStartY, laserEndX, laserEndY, middleX, top, middleX, bottom)) {
		isHit = 1;
		}
	//��
	if (LineLine(laserStartX, laserStartY, laserEndX, laserEndY, left, middleY, right, middleY)) {
		isHit = 1;
		}

	return isHit;
	}

//���[�U�[�Ƌ�
int LineMirror(float laserStartX, float laserStartY, float* laserEndX, float* laserEndY, MAP map, int x, int y) {
	float top = y * 32, bottom = y * 32 + 32;
	float left = x * 32, right = x * 32 + 32;
	if (map.CHIP[map.stage][y][x] == 11) {
		int isHit = 0;
		//��
		if (LineLine(laserStartX, laserStartY, laserEndX, laserEndY, left, top, right, top)) {
			isHit = 1;
			}
		//��
		if (LineLine(laserStartX, laserStartY, laserEndX, laserEndY, left, bottom, right, bottom)) {
			isHit = 1;
			}
		//��
		if (LineLine(laserStartX, laserStartY, laserEndX, laserEndY, left, top, left, bottom)) {
			isHit = 1;
			}
		//�E
		if (LineLine(laserStartX, laserStartY, laserEndX, laserEndY, right, top, right, bottom)) {
			isHit = 1;
			}
		return isHit;
		}
	if (map.CHIP[map.stage][y][x] == 12) {
		LineLine(laserStartX, laserStartY, laserEndX, laserEndY, left, top, right, top);
		LineLine(laserStartX, laserStartY, laserEndX, laserEndY, left, top, left, bottom);
		if (LineLine(laserStartX, laserStartY, laserEndX, laserEndY, left, bottom, right, top) == 1) {
			return 1;
			}
		return 2;
		}
	if (map.CHIP[map.stage][y][x] == 13) {
		LineLine(laserStartX, laserStartY, laserEndX, laserEndY, left, top, right, top);
		LineLine(laserStartX, laserStartY, laserEndX, laserEndY, right, top, right, bottom);
		if (LineLine(laserStartX, laserStartY, laserEndX, laserEndY, right, bottom, left, top) == 1) {
			return 1;
			}
		return 2;
		}
	if (map.CHIP[map.stage][y][x] == 14) {
		LineLine(laserStartX, laserStartY, laserEndX, laserEndY, left, bottom, right, bottom);
		LineLine(laserStartX, laserStartY, laserEndX, laserEndY, right, top, right, bottom);
		if (LineLine(laserStartX, laserStartY, laserEndX, laserEndY, left, bottom, right, top) == 1) {
			return 1;
			}
		return 2;
		}
	if (map.CHIP[map.stage][y][x] == 15) {
		LineLine(laserStartX, laserStartY, laserEndX, laserEndY, left, bottom, right, bottom);
		LineLine(laserStartX, laserStartY, laserEndX, laserEndY, left, top, left, bottom);
		if (LineLine(laserStartX, laserStartY, laserEndX, laserEndY, right, bottom, left, top) == 1) {
			return 1;
			}
		return 2;
		}
	return 0;
	}