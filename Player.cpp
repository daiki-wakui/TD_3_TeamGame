#include"Player.h"
#include"DxLib.h"
#include"Colision.h"
#include "Map.h"

//������
Player::Player() {
	idleTimer = 0;
	if (map.stage == 0) {
		transform.posX = 288;
		transform.posY = 864;
	}
	else if (map.stage == 1) {
		transform.posX = 352;
		transform.posY = 864;
	}
	else if (map.stage == 2) {
		transform.posX = 32;
		transform.posY = 128;
	}
	else if (map.stage == 3) {
		transform.posX = 1280;
		transform.posY = 672;
	}

	transform.r = 32;
	Pic = LoadGraph("player.png");
	LoadDivGraph("Character_idle.png", 7, 7, 1, 72, 72, idlePic, true);
	LoadDivGraph("player_glide.png", 2, 2, 1, 64, 64, glidePic, true);
	walkSpeed = 6;
	dashSpeed = 12;
	isJump = 0;
	jumpSpeed = -25;
	jumpTimer = 0;
	speedTmp = 2;
	isGliding = 0;
	glidingG = 0;
	isGliderColliding = 0;
	hasGlided = 0;
	direction = 0;
	state = 0;
	hitSide = transform.r - 12;
	//�d��
	g = 0;
	//���[�U�[
	isMove = 0;
	//�}�b�v�`�b�v���W
	leftTopX = 0;
	leftTopY = 0;
	leftUnderX = 0;
	leftUnderY = 0;
	rightTopX = 0;
	rightTopY = 0;
	rightUnderX = 0;
	rightUnderY = 0;
	rightCentralX = 0;
	rightCentralY = 0;
	leftCentralX = 0;
	leftCentralY = 0;
	CentralY = 0;
	CentralX = 0;

	anime = new Animation;
}

void Player::Reset() {
	*this = {};
}

//�l���Ǝl���̒��Ԃ̃}�b�v�`�b�v���W�̎擾
void Player::MapchipInfo() {
	rightTopX = (transform.posX + hitSide - 1) / 32;
	rightUnderX = (transform.posX + hitSide - 1) / 32;
	rightCentralX = (transform.posX + hitSide - 1) / 32;
	leftCentralX = (transform.posX - hitSide - 1) / 32;
	leftTopX = (transform.posX - hitSide) / 32;
	leftUnderX = (transform.posX - hitSide) / 32;

	rightTopY = (transform.posY - transform.r) / 32;
	rightUnderY = (transform.posY + transform.r - 1) / 32;
	rightCentralY = (transform.posY - 1) / 32;
	leftCentralY = (transform.posY - 1) / 32;
	leftTopY = (transform.posY - transform.r) / 32;
	leftUnderY = (transform.posY + transform.r - 1) / 32;
	}

//���ړ������Ƃ��̃}�b�v�`�b�v���W�̎擾
void Player::MapchipTemporaryInfo() {
	rightTopX = (transform.posX + hitSide - 1 + speedTmp) / 32;
	rightUnderX = (transform.posX + hitSide - 1 + speedTmp) / 32;
	rightCentralX = (transform.posX + hitSide - 1 + speedTmp) / 32;
	leftTopX = (transform.posX - hitSide - speedTmp) / 32;
	leftUnderX = (transform.posX - hitSide - speedTmp) / 32;
	leftCentralX = (transform.posX - hitSide - 1 - speedTmp) / 32;
}

//Move�֐�	(�R���g���[���[)
void Player::Move(XINPUT_STATE pad, XINPUT_STATE padOld) {
	MapchipInfo();
	dashSpeed = 0;
	walkSpeed = 0;
	//�d��
	if (isGliding == false) {
		if (g >= 60) {
			g = 60;
		}
		g += 2;
		transform.posY += g;
		isGliderColliding = false;
	}

	if (isMove == 1) {

		//�E�ړ�
		if (pad.ThumbLX > 9000 && isGliding == false) {
			idleTimer = 0;
			direction = RIGHT;
			//�_�b�V��
			if (pad.RightTrigger > 150) {
				dashSpeed = 8;
				speedTmp = dashSpeed + 3;
			}
			//�ʏ�
			else {
				walkSpeed = 6;
				speedTmp = walkSpeed + 3;
			}
			MapchipTemporaryInfo();
			//�}�b�v�`�b�v�����蔻��
			for (int color = 1; color < 8; color++) {
				if (map.CHIP[map.stage][rightTopY][rightTopX] == color ||
					map.CHIP[map.stage][rightUnderY][rightUnderX] == color ||
					map.CHIP[map.stage][rightCentralY][rightCentralX] == color) {
					//hitSide = 20;
					if (map.isDraw[color] == true) {
						if (transform.posX + hitSide < rightTopX * 32) {
							if (pad.RightTrigger > 150) {
								dashSpeed = (rightTopX * 32) - (transform.posX + hitSide);
								}
							else {
								walkSpeed = (rightTopX * 32) - (transform.posX + hitSide);
								}
							}
						else {
							walkSpeed = 0;
							dashSpeed = 0;
						}
					}
				}
			}
		}
		//���ړ�
		if (pad.ThumbLX < -9000 && isGliding == false) {
			idleTimer = 0;
			direction = LEFT;
			//�_�b�V��
			if (pad.RightTrigger > 150) {
				dashSpeed = -8;
				speedTmp = -dashSpeed;
			}
			//�ʏ�
			else {
				walkSpeed = -6;
				speedTmp = -walkSpeed;
			}

			MapchipTemporaryInfo();
			//�}�b�v�`�b�v�����蔻��
			for (int color = 1; color < 8; color++) {
				if (map.CHIP[map.stage][leftTopY][leftTopX] == color ||
					map.CHIP[map.stage][leftUnderY][leftUnderX] == color ||
					map.CHIP[map.stage][leftCentralY][leftCentralX] == color) {

					if (map.isDraw[color] == true) {
						if (transform.posX - hitSide < leftTopX * 32) {
							if (pad.RightTrigger > 150) {
								dashSpeed = -(leftTopX * 32) - (transform.posX + hitSide);
							}
							else {
								walkSpeed = -(leftTopX * 32) - (transform.posX + hitSide);
							}
						}
						else {
							walkSpeed = 0;
							dashSpeed = 0;
							speedTmp = 6;
						}
					}
				}
			}
		}

		//�W�����v
		if (pad.Buttons[XINPUT_BUTTON_A] == 1 && padOld.Buttons[XINPUT_BUTTON_A] == 0 && isJump == 0) {
			jumpSpeed = -25;
			MapchipInfo();
			rightTopY = (transform.posY - transform.r - 16) / 32;
			leftTopY = (transform.posY - transform.r - 16) / 32;

			CentralX = transform.posX / 32;

			for (int color = 1; color < 8; color++) {
				CentralY = (transform.posY + transform.r) / 32;
				if (map.CHIP[map.stage][leftUnderY][leftUnderX] == color ||
					map.CHIP[map.stage][rightUnderY][rightUnderX] == color ||
					map.CHIP[map.stage][CentralY][CentralX] == color) {

					if (map.isDraw[color] == true && hasGlided == false) {
						isJump = 1;
						}
					}
				CentralY = (transform.posY - transform.r - 16) / 32;
				if (map.CHIP[map.stage][leftTopY][leftTopX] == color ||
					map.CHIP[map.stage][rightTopY][rightTopX] == color ||
					map.CHIP[map.stage][CentralY][CentralX] == color) {

					if (map.isDraw[color] == true) {
						isJump = 0;
						}
					}
				}

			if (isGliding == true) {
				isJump = 0;
			}
		}

		if (isJump == 1 && isGliding == false) {
			transform.posY += jumpSpeed;
			idleTimer = 0;
		}
	}

	if (dashSpeed == 0 && walkSpeed == 0) {
		state = IDLE;
		idleTimer++;
	}
	else if (walkSpeed == 6 || walkSpeed == -6) {
		state = WALK;
	}
	else if (dashSpeed == 8 || dashSpeed == -8) {
		state = DASH;
	}

	//�O���C�_�[
	if (pad.LeftTrigger > 150 && g >= 25) {
		if (isGliderColliding == false && hasGlided == false) {
			isGliding = true;
			}
		}
	else {
		isGliding = false;
		}

	if (isGliding == true) {
		idleTimer = 0;
		if (pad.ThumbLX > 4500) {
			direction = RIGHT;
			}
		if (pad.ThumbLX < -4500) {
			direction = LEFT;
			}
		g = 25;
		glidingG = 2;
		isJump = 0;
		//�E
		if (direction == RIGHT) {
			walkSpeed = 1;
			dashSpeed = 1;
			speedTmp = 2;

			if (pad.ThumbLX > 17000) {
				walkSpeed = 2;
				dashSpeed = 2;
				speedTmp = 4;
			}
			if (pad.ThumbLX > 30000) {
				walkSpeed = 4;
				dashSpeed = 4;
				speedTmp = 8;
			}
		}

		if (direction == LEFT) {
			walkSpeed = -1;
			dashSpeed = -1;
			speedTmp = 2;
			//��
			if (pad.ThumbLX < -17000) {
				walkSpeed = -2;
				dashSpeed = -2;
				speedTmp = 4;
				}
			if (pad.ThumbLX < -30000) {
				walkSpeed = -4;
				dashSpeed = -4;
				speedTmp = 8;
				}
			}


		MapchipTemporaryInfo();
		//�}�b�v�`�b�v�����蔻��
		for (int color = 1; color < 8; color++) {
			if (map.CHIP[map.stage][rightTopY][rightTopX] == color ||
				map.CHIP[map.stage][rightUnderY][rightUnderX] == color ||
				map.CHIP[map.stage][rightCentralY][rightCentralX] == color ||
				map.CHIP[map.stage][leftTopY][leftTopX] == color ||
				map.CHIP[map.stage][leftUnderY][leftUnderX] == color ||
				map.CHIP[map.stage][leftCentralY][leftCentralX] == color) {

				if (map.isDraw[color] == true) {
					walkSpeed = 0;
					dashSpeed = 0;
					isGliderColliding = true;
				}
			}
		}

		if (isGliderColliding == true) {
			isGliding = false;
			hasGlided = true;
			g = 0;
		}

		transform.posY += glidingG;
	}
	else if (pad.LeftTrigger < 150 && g == 25) {
		isGliding = false;
		g = 0;
	}

	for (int y = 0; y < 30; y++) {
		for (int x = 0; x < 44; x++) {
			for (int color = 1; color < 8; color++) {
				if (map.isDraw[color] == 1 || map.CHIP[map.stage][y][x] == 1) {
					if (Colision(transform, map, x, y, hitSide) == color) {
						//�n��
						if (transform.posY + transform.r < (32 * y) + (5 + g)) {
							transform.posY += (32 * y) - (transform.posY + transform.r);
							hasGlided = false;
							isJump = 0;
							g = 0;
						}
					}
					if (Colision(transform, map, x, y, hitSide) == color) {
						//�V��
						if (transform.posY - transform.r > 32 + (32 * y) - (21 + g)) {
							transform.posY += 32 + (32 * y) - (transform.posY - transform.r);
							jumpSpeed = 1;
							g = 0;
						}
					}
				}
			}
		}
	}

	transform.posX += walkSpeed;
	transform.posX += dashSpeed;
}

//Draw�֐�
void Player::Draw() {
	/*DrawBox(transform.posX - hitSide, transform.posY - transform.r,
		transform.posX + hitSide, transform.posY + transform.r,
		GetColor(255, 255, 255), false);

	DrawBox(transform.posX - transform.r, transform.posY - transform.r,
		transform.posX + transform.r, transform.posY + transform.r,
		GetColor(255, 0, 0), false);*/

	//�ړ����ĂȂ��Ƃ�
	if (state == IDLE) {

		anime->animation(anime->playerFrame, anime->playerTimer, 4, 7);
		//45�t���[���̊Ԃ͐Î~��B�o�ߌ�A�j���[�V����
		if (idleTimer > 45) {	//�A�j���[�V����
			//�E
			if (direction == RIGHT && isGliding == false) {
				DrawGraph(
					transform.posX - transform.r,
					transform.posY - transform.r - 8,
					idlePic[anime->playerTimer], true);
			}
			//��
			else if (direction == LEFT && isGliding == false) {
				DrawTurnGraph(
					transform.posX - transform.r - 8,
					transform.posY - transform.r - 8,
					idlePic[anime->playerTimer], true);
			}
		}
		else {	//�Î~��
			if (direction == RIGHT && isGliding == false) {
				DrawGraph(
					transform.posX - transform.r,
					transform.posY - transform.r - 8,
					idlePic[0], true);
			}
			else if (direction == LEFT && isGliding == false) {
				DrawTurnGraph(
					transform.posX - transform.r - 8,
					transform.posY - transform.r - 8,
					idlePic[0], true);
			}
		}
		
	}
	//�ړ����Ă�Ƃ�
	if (state == WALK) {
		anime->animation(anime->playerFrame, anime->playerTimer, 4, 7);

		if (direction == RIGHT && isGliding == false) {
			DrawGraph(
				transform.posX - transform.r,
				transform.posY - transform.r - 8,
				idlePic[anime->playerTimer], true);
		}
		else if (direction == LEFT && isGliding == false) {
			DrawTurnGraph(
				transform.posX - transform.r - 8,
				transform.posY - transform.r - 8,
				idlePic[anime->playerTimer], true);
		}
	}
	//�_�b�V���ňړ����Ă�Ƃ�
	if (state == DASH) {
		anime->animation(anime->playerFrame, anime->playerTimer, 4, 7);

		if (direction == RIGHT && isGliding == false) {
			DrawGraph(
				transform.posX - transform.r,
				transform.posY - transform.r - 8,
				idlePic[anime->playerTimer], true);
		}
		else if (direction == LEFT && isGliding == false) {
			DrawTurnGraph(
				transform.posX - transform.r - 8,
				transform.posY - transform.r - 8,
				idlePic[anime->playerTimer], true);
		}
	}

	//�O���C�_�[�`��
	if (isGliding == true) {
		//�E����
		if (direction == RIGHT) {
			/*DrawTriangle(
				transform.posX - transform.r, transform.posY - transform.r,
				transform.posX - transform.r, transform.posY - transform.r * 3,
				transform.posX + transform.r, transform.posY - transform.r * 2,
				GetColor(255, 255, 255), true);*/
			DrawGraph(transform.posX - transform.r, transform.posY - transform.r, glidePic[0], true);
			}
		//������
		else if (direction == LEFT) {
			/*DrawTriangle(
				transform.posX + transform.r, transform.posY - transform.r,
				transform.posX + transform.r, transform.posY - transform.r * 3,
				transform.posX - transform.r, transform.posY - transform.r * 2,
				GetColor(255, 255, 255), true);*/
			DrawGraph(transform.posX - transform.r, transform.posY - transform.r, glidePic[1], true);
			}
		}
	//�f�o�b�N�`��
	/*if (isMove == 0) {
		DrawBox(
			transform.posX - transform.r, transform.posY - transform.r,
			transform.posX + transform.r, transform.posY + transform.r,
			GetColor(125, 125, 125), true);
	}*/
}