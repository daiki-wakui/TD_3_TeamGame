#include "UI.h"
#include "DxLib.h"
#include "Sturct.h"
#include "Map.h"
#include"Colision.h"
#include "Animation.h"

//�R���X�g���N�^�̒�`
UI::UI() {
	LoadDivGraph("b.png", 2, 2, 1, 32, 32, bButtanPic, true);
	LoadDivGraph("Rstick.png", 8, 8, 1, 64, 64, rStickPic, true);
	pauseImage = LoadGraph("poss.png");
	ControlImage = LoadGraph("Control.png");
	xBoxImage = LoadGraph("XBox.png");
	magnification = 0;
	buttan = 0;
	isContorolMethod = 0;
	ContorolMagnification = 0;
	isEndPause = 0;
	fade = 0;
	isIn = 0;
	isOut = 0;
	isChange = 0;
	returnTitle = 0;
	direction = 0;

	anime = new Animation;
}

//�V�[���؂�ւ��C���A�E�g
void UI::SceneTransition() {
	//�X�V����
	if (isChange == true) {
		if (isIn == true) {

			fade += 15;
			if (fade >= 235) {
				isOut = true;
				isIn = false;
			}
		}

		if (isOut == true) {
			fade -= 10;

			if (fade <= 0) {
				fade = 0;
				isOut = false;
				isIn = false;
				isChange = false;
			}
		}
	}
	
}

void UI::DrawFadeInOut() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade);
	DrawBox(0, 0, WIN_WIDTH, WIN_HEIGHT, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//�|�[�Y���̏���
void UI::PauseUpdate(XINPUT_STATE pad, XINPUT_STATE padOld, int& scene, int& reset) {
	//�|�[�Y�̃E�B���h�E�����
	if (isEndPause == true) {
		magnification -= 0.2;

		if (magnification < 0.0) {
			magnification = 0.0;
		}
	}
	//�|�[�Y��ʂ�\����
	else {
		magnification += 0.2;

		if (magnification > 1.0) {
			magnification = 1.0;
		}
	}
	//�I��
	if (pad.ThumbLY > 9000 && padOld.ThumbLY < 9000) {
		if (buttan != 3) {
			buttan++;
		}
	}
	else if (pad.ThumbLY < -9000 && padOld.ThumbLY>-9000) {
		if (buttan != 0) {
			buttan--;
		}
	}

	//������@��ʂ�Y����������
	if (pad.Buttons[XINPUT_BUTTON_Y] == 1 && padOld.Buttons[XINPUT_BUTTON_Y] == 0 && isContorolMethod == true) {
		isContorolMethod = false;
	}
	//�|�[�Y��ʂ�Y����������
	else if (pad.Buttons[XINPUT_BUTTON_Y] == 1 && padOld.Buttons[XINPUT_BUTTON_Y] == 0 && isContorolMethod == false) {
		isEndPause = true;	//�|�[�Y�I��
	}

	//�|�[�Y����A�j���[�V�������I�������
	if (magnification <= 0.0) {
		scene = 2;	//�Q�[����ʂɖ߂�
		isContorolMethod = false;
		isEndPause = false;
	}

	//����
	if (pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0) {
		//������@�m�F
		if (buttan == 2) {
			isContorolMethod = true;	//������@��ʂ��J��
		}
		if (isChange == false && isContorolMethod == false) {
			isChange = true;
			isIn = true;
		}
	}
	//�^�C�g����ʂ�
	if (buttan == 0) {
		if (fade >= 225) {
			returnTitle = true;
			scene = 0;
		}
	}
	//�X�e�[�W��ʑI����
	else if (buttan == 1) {
		if (fade >= 225) {
			scene = 1;
		}
	}
	else if (buttan == 3) {

		if (fade >= 225) {
			reset = 1;
			scene = 2;
		}
	}

	//������@��ʂ��J���Ă���
	if (isContorolMethod == true && isEndPause == false) {
		ContorolMagnification += 0.2;

		if (ContorolMagnification > 1.4) {
			ContorolMagnification = 1.4;
		}
	}
	//������@��ʂ����
	else if (isContorolMethod == false) {
		ContorolMagnification -= 0.2;

		if (ContorolMagnification < 0.0) {
			ContorolMagnification = 0.0;
		}
	}
}

void UI::ButtonDraw(XINPUT_STATE pad, XINPUT_STATE padOld,TransForm player) {
	for (int y = 0; y < 30; y++) {
		for (int x = 0; x < 44; x++) {
			if (map.CHIP[map.stage][y][x] == R_LASER) {
				pointX[0] = x * 32 + 16;
				pointY[0] = y * 32 + 16;
			}

			if (map.CHIP[map.stage][y][x] == G_LASER) {
				pointX[1] = x * 32 + 16;
				pointY[1] = y * 32 + 16;
			}

			if (map.CHIP[map.stage][y][x] == B_LASER) {
				pointX[2] = x * 32 + 16;
				pointY[2] = y * 32 + 16;
			}
		}
	}
	//����
	if (pad.ThumbLX > 9000 && isLaserMode == 0) {
		direction = RIGHT;
	}

	if (pad.ThumbLX < -9000 && isLaserMode == 0) {
		direction = LEFT;
	}

	if (pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0) {
		if (isLaserMode == 0) {
			isLaserMode = 1;
		}
		else {
			isLaserMode = 0;
		}
	}

	if (pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0 ||
		pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 1) {
		isPush = 1;
	}
	else {
		isPush = 0;
	}

	anime->animation(anime->rStickFrame, anime->rStickTimer, 7, 8);
	//�`�揈��
	//�����ɂ���ĕω�
	if (direction == RIGHT) {
		for (int i = 0; i < 3; i++) {
			if (isPush == 1 && Colision(player, pointX[i], pointY[i])) {

				DrawGraph(
					player.posX + player.r,
					player.posY - player.r - 16,
					bButtanPic[1], true);
			}
			else if (Colision(player, pointX[i], pointY[i])) {
				DrawGraph(
					player.posX + player.r,
					player.posY - player.r - 16,
					bButtanPic[0], true);

				if (isLaserMode == true) {
					DrawGraph(
						player.posX - player.r * 3,
						player.posY - player.r - 32,
						rStickPic[anime->rStickTimer], true);
				}
			}
		}

		
	}
	else if (direction == LEFT) {
		for (int i = 0; i < 3; i++) {
			if (isPush == 1&& Colision(player, pointX[i], pointY[i])) {

				DrawGraph(
					player.posX - player.r * 2,
					player.posY - player.r - 16,
					bButtanPic[1], true);
			}
			else if (Colision(player, pointX[i], pointY[i])) {
				DrawGraph(
					player.posX - player.r * 2,
					player.posY - player.r - 16,
					bButtanPic[0], true);

				if (isLaserMode == true) {
					DrawGraph(
						player.posX + player.r,
						player.posY - player.r - 32,
						rStickPic[anime->rStickTimer], true);
				}
			}
		}	
	}
	
	DrawFormatString(0, 0, GetColor(255, 255, 255), "muki %d", direction);
}

//�|�[�Y���̕`��֐�
void UI::PauseDraw() {
	////----�|�[�Y��ʕ`��----////
	DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, magnification, 0.0, pauseImage, true);

	////----������@�̉�ʕ`��----////
	DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, ContorolMagnification, 0.0, ControlImage, true);
	DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, ContorolMagnification, 0.0, xBoxImage, true);
	

	//�f�o�b�O�p
	DrawFormatString(500, 430, GetColor(255, 255, 255), "�|�[�Y���");
	DrawFormatString(500, 500, GetColor(255, 255, 255), "L�X�e�B�b�N�őI��");
	if (buttan == 0) {
		DrawFormatString(500, 530, GetColor(255, 255, 255), "B�Ń^�C�g����ʂ�");
	}
	else if (buttan == 1) {
		DrawFormatString(500, 530, GetColor(255, 255, 255), "B�ŃX�e�[�W�I����ʂ�");
	}
	else if (buttan == 2) {
		DrawFormatString(500, 530, GetColor(255, 255, 255), "B�ő�����@��");
		DrawFormatString(500, 550, GetColor(255, 255, 255), "�t��[�ށ@%f", ContorolMagnification);
	}
	else if (buttan == 3) {
		DrawFormatString(500, 530, GetColor(255, 255, 255), "B�ŃX�^�[�g�����蒼��");
	}

	
}


void UI::drawColorPalette(int x, int y, int* graph) {
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	if (map.isDraw[RED] == false && map.isDraw[GREEN] == false && map.isDraw[BLUE] == false) {
		SetDrawBright(100, 0, 0);
		DrawGraph(x, y, graph[0], true);
		SetDrawBright(0, 100, 0);
		DrawGraph(x + 32, y, graph[1], true);
		SetDrawBright(0, 0, 100);
		DrawGraph(x + 16, y + 32, graph[2], true);
		}
	else if (map.isDraw[RED] == true && map.isDraw[GREEN] == false && map.isDraw[BLUE] == false) {
		SetDrawBright(255, 0, 0);
		DrawGraph(x, y, graph[0], true);
		SetDrawBright(0, 100, 0);
		DrawGraph(x + 32, y, graph[1], true);
		SetDrawBright(0, 0, 100);
		DrawGraph(x + 16, y + 32, graph[2], true);
		}
	else if (map.isDraw[RED] == false && map.isDraw[GREEN] == true && map.isDraw[BLUE] == false) {
		SetDrawBright(100, 0, 0);
		DrawGraph(x, y, graph[0], true);
		SetDrawBright(0, 255, 0);
		DrawGraph(x + 32, y, graph[1], true);
		SetDrawBright(0, 0, 100);
		DrawGraph(x + 16, y + 32, graph[2], true);
		}
	else if (map.isDraw[RED] == false && map.isDraw[GREEN] == false && map.isDraw[BLUE] == true) {
		SetDrawBright(100, 0, 0);
		DrawGraph(x, y, graph[0], true);
		SetDrawBright(0, 100, 0);
		DrawGraph(x + 32, y, graph[1], true);
		SetDrawBright(0, 0, 255);
		DrawGraph(x + 16, y + 32, graph[2], true);
		}
	else if (map.isDraw[RED] == true && map.isDraw[GREEN] == true && map.isDraw[BLUE] == false) {
		SetDrawBright(255, 0, 0);
		DrawGraph(x, y, graph[0], true);
		SetDrawBright(0, 255, 0);
		DrawGraph(x + 32, y, graph[1], true);
		SetDrawBright(0, 0, 100);
		DrawGraph(x + 16, y + 32, graph[2], true);
		}
	else if (map.isDraw[RED] == true && map.isDraw[GREEN] == true && map.isDraw[BLUE] == true) {
		SetDrawBright(255, 0, 0);
		DrawGraph(x, y, graph[0], true);
		SetDrawBright(0, 255, 0);
		DrawGraph(x + 32, y, graph[1], true);
		SetDrawBright(0, 0, 255);
		DrawGraph(x + 16, y + 32, graph[2], true);
		}
	else if (map.isDraw[RED] == true && map.isDraw[GREEN] == false && map.isDraw[BLUE] == true) {
		SetDrawBright(255, 0, 0);
		DrawGraph(x, y, graph[0], true);
		SetDrawBright(0, 100, 0);
		DrawGraph(x + 32, y, graph[1], true);
		SetDrawBright(0, 0, 255);
		DrawGraph(x + 16, y + 32, graph[2], true);
		}
	else if (map.isDraw[RED] == false && map.isDraw[GREEN] == true && map.isDraw[BLUE] == true) {
		SetDrawBright(100, 0, 0);
		DrawGraph(x, y, graph[0], true);
		SetDrawBright(0, 255, 0);
		DrawGraph(x + 32, y, graph[1], true);
		SetDrawBright(0, 0, 255);
		DrawGraph(x + 16, y + 32, graph[2], true);
		}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	SetDrawBright(255, 255, 255);
	}