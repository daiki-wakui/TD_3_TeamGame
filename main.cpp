#include "DxLib.h"
#include "Player.h"
#include "Laser.h"
#include "Map.h"
#include"Colision.h"
#include "UI.h"
#include "Sturct.h"


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
	{
	// �E�B���h�E���[�h�ɐݒ�
	ChangeWindowMode(TRUE);

	// �E�B���h�E�T�C�Y���蓮�ł͕ύX�������A
	// ���E�B���h�E�T�C�Y�ɍ��킹�Ċg��ł��Ȃ��悤�ɂ���
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// �^�C�g����ύX
	SetMainWindowText(TITLE);

	// ��ʃT�C�Y�̍ő�T�C�Y�A�J���[�r�b�g����ݒ�(���j�^�[�̉𑜓x�ɍ��킹��)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// ��ʃT�C�Y��ݒ�(�𑜓x�Ƃ̔䗦�Őݒ�)
	SetWindowSizeExtendRate(1.0);

	// ��ʂ̔w�i�F��ݒ肷��
	SetBackgroundColor(120, 120, 120);

	// DXlib�̏�����
	if (DxLib_Init() == -1) { return -1; }

	// (�_�u���o�b�t�@)�`���O���t�B�b�N�̈�͗��ʂ��w��
	SetDrawScreen(DX_SCREEN_BACK);

	// �摜�Ȃǂ̃��\�[�X�f�[�^�̕ϐ��錾�Ɠǂݍ���
	int backHandle = LoadGraph("background.png");

	LoadDivGraph("block.png", 16, 4, 4, 32, 32, map.chipBlock, true);
	map.crystalPic = LoadGraph("crystal.png");
	int colorPalette[3];
	LoadDivGraph("color_pallette.png", 3, 3, 1, 64, 64, colorPalette, true);
	int gauss = MakeScreen(WIN_WIDTH, WIN_HEIGHT, true);
	
	
	// �Q�[�����[�v�Ŏg���ϐ��̐錾
	int scene = 0;
	int isClear = 0;
	int reset = 0;
	int isSceneChange = 0;

	int stage = 0;
	int stageMax = 4;

	int time = 0;
	int ms = 0;
	int second = 0;
	int minute = 0;

	int blurHandle = MakeScreen(WIN_WIDTH, WIN_HEIGHT);
	//------------------�R���g���[���[�ϐ��錾-----------------------//
	XINPUT_STATE pad;
	for (int i = 0; i < 16; i++) {
		pad.Buttons[i] = 0;
		}
	pad.ThumbLX = 0;
	pad.ThumbLY = 0;
	pad.ThumbRX = 0;
	pad.ThumbRY = 0;
	XINPUT_STATE padOld;
	SetJoypadDeadZone(DX_INPUT_PAD1, 1);

	//------------------�v���C���[������--------------//
	Player* player = new Player();

	//-------------------���[�U�[������--------------//
	Laser* redLaser = new Laser(map, R_LASER);
	Laser* greenLaser = new Laser(map, G_LASER);
	Laser* blueLaser = new Laser(map, B_LASER);

	//--------------�|�[�Y��ʃC���X�^���X����----------//
	UI* ui = new UI;

	// �ŐV�̃L�[�{�[�h���p
	char keys[256] = { 0 };

	// 1���[�v(�t���[��)�O�̃L�[�{�[�h���
	char oldkeys[256] = { 0 };

	// �Q�[�����[�v
	while (1)
		{
		// �ŐV�̃L�[�{�[�h��񂾂������̂�1�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
		for (int i = 0; i < 256; ++i)
			{
			oldkeys[i] = keys[i];
			}
		// �ŐV�̃L�[�{�[�h�����擾
		GetHitKeyStateAll(keys);

		// ��ʃN���A
		ClearDrawScreen();
		//---------  ��������v���O�������L�q  ----------//

		//--------------------------------------------------�X�V����----------------------------------------------------------//

		//�R���g���[���[����
		for (int i = 0; i < 16; i++) {
			padOld.Buttons[i] = pad.Buttons[i];
			}
		padOld.ThumbLX = pad.ThumbLX;
		padOld.ThumbLY = pad.ThumbLY;
		padOld.ThumbRX = pad.ThumbRX;
		padOld.ThumbRY = pad.ThumbRY;
		GetJoypadXInputState(DX_INPUT_PAD1, &pad);


		//�V�[��
		switch (scene) {
				case 0://�^�C�g�����---------------------------------------------------
					if (ui->isChange == false) {
						ui->returnTitle = false;
					}
					//�X�e�[�W�I����
					if (pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0) {

						if (ui->isChange == false) {
							ui->isChange = true;
							ui->isIn = true;
						}
					}

					if (ui->isChange == true) {
						ui->SceneTransition();
					}

					if (ui->isIn == false && ui->isChange == true && ui->returnTitle == false) {
						scene = 1;
					}

					break;

				case 1://�X�e�[�W�I�����-----------------------------------------------

					if (ui->isChange == true) {
						ui->SceneTransition();
					}

					//�X�e�[�W�I��
					if (pad.ThumbLX > 9000 && padOld.ThumbLX < 9000) {
						if (map.stage != stageMax) {
							map.stage++;
						}
					}
					else if (pad.ThumbLX < -9000 && padOld.ThumbLX > -9000) {
						if (map.stage != 0) {
							map.stage--;
						}
					}

					//�Q�[����ʂ�
					if (pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0) {
						if (ui->isChange == false) {
							ui->isChange = true;
							ui->isIn = true;
						}
					}

					if (ui->isChange == true) {
						ui->SceneTransition();
					}

					if (ui->fade >= 225) {
						scene = 2;
						reset = 1;
					}

					break;

				case 2://�Q�[�����------------------------------------------------------

					if (ui->isChange == true) {
						ui->SceneTransition();
					}

					if (isClear == 0) {

						time += 20;
						ms = time / 10;

						if (time >= 1000) {
							time = 0;
							second++;
							ms = 0;
						}

						if (ms >= 100) {
							second++;
							ms = 0;
						}
						if (second >= 60) {
							minute++;
							second = 0;
						}

						//�|�[�Y��ʂ�
						if (pad.Buttons[XINPUT_BUTTON_Y] == 1 && padOld.Buttons[XINPUT_BUTTON_Y] == 0) {
							scene = 3;
							DeleteGraph(gauss);
						}

						////////////////�v���C���[///////////////
						//�ړ�
						player->isMove = 1;
						if (redLaser->isAngle == 1 || blueLaser->isAngle == 1 || greenLaser->isAngle == 1) {
							player->isMove = 0;
						}
						else {
							player->isMove = 1;
						}
						player->Move(pad, padOld);

						///////////////���[�U�[////////////////
						redLaser->Angle(pad, padOld, player->transform);
						greenLaser->Angle(pad, padOld, player->transform);
						blueLaser->Angle(pad, padOld, player->transform);

						//�N���A����
						if (map.isDraw[RED] == 1 && map.isDraw[GREEN] == 1 && map.isDraw[BLUE] == 1) {
							isClear = 1;
							}
						}
					//�N���A��
					else if (isClear == 1) {
						if (pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0) {
							scene = 0;
						}
					}

					if (player->transform.posY >= WIN_HEIGHT) {
						scene = 4;
					}

					ui->magnification = 0;
					break;

				case 3://�|�[�Y���------------------------------------------------------
				
					if (ui->isChange == true) {
						ui->SceneTransition();
					}

					ui->PauseUpdate(pad, padOld, scene, reset);

					break;

				case 4:
					if (pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0) {
						player->Reset();
						delete redLaser;
						delete greenLaser;
						delete blueLaser;
						player = new Player();
						redLaser = new Laser(map, R_LASER);
						greenLaser = new Laser(map, G_LASER);
						blueLaser = new Laser(map, B_LASER);
						scene = 2;
					}

					break;
			}

		//������
		if (reset == 1) {
			reset = 0;
			player->Reset();
			redLaser = new Laser(map, R_LASER);
			greenLaser = new Laser(map, G_LASER);
			blueLaser = new Laser(map, B_LASER);
			for (int i = 2; i < 6; i++) {
				map.isDraw[i] = 0;
			}
			isClear = 0;
			time = 0;
			second = 0;
			ms = 0;
		}

		//-------------------------------------------------�`�揈��------------------------------------------------------------------//

		//�V�[��
		switch (scene) {
				case 0://�^�C�g��
					if (ui->isChange == true) {
						ui->DrawFadeInOut();
					}
					
				//�f�o�b�O�p
					DrawFormatString(500, 430, GetColor(255, 255, 255), "�^�C�g�����");
					DrawFormatString(500, 530, GetColor(255, 255, 255), "B�ŃX�e�[�W�I����");				

					break;

				case 1://�X�e�[�W�I�����---------------------------------------------------
					if (ui->isChange == true) {
						ui->DrawFadeInOut();
					}
				//�f�o�b�O�p
					DrawFormatString(500, 430, GetColor(255, 255, 255), "�X�e�[�W�I�����");
					DrawFormatString(500, 500, GetColor(255, 255, 255), "L�X�e�B�b�N�őI��");
					DrawFormatString(500, 530, GetColor(255, 255, 255), "B�ŃX�e�[�W%d��", map.stage + 1);
					break;

				case 2://�Q�[�����----------------------------------------------------------

					DrawGraph(0, 0, backHandle, true);
					redLaser->Draw();
					greenLaser->Draw();
					blueLaser->Draw();
					Map();
					player->Draw();
					ui->ButtonDraw(pad,padOld,player->transform);
					ui->drawColorPalette(1200, 60, colorPalette);

					if (pad.Buttons[XINPUT_BUTTON_Y] == 1 && padOld.Buttons[XINPUT_BUTTON_Y] == 0) {
						GetDrawScreenGraph(0, 0, WIN_WIDTH, WIN_HEIGHT, gauss);
					}
					DrawFormatString(1300, 100, GetColor(220, 220, 220), "%02d:%02d:%02d", minute, second, ms);

					if (isClear == 1) {
						SetFontSize(64);
						DrawFormatString(650, 400, GetColor(220, 220, 220), "CLEAR");
						DrawFormatString(500, 530, GetColor(255, 255, 255), "B�Ń^�C�g����ʂ�");
						SetFontSize(16);
					}

					if (ui->isChange == true) {
						ui->DrawFadeInOut();
					}
					//�f�o�b�O�p
					DrawFormatString(20, 800, GetColor(255, 255, 255), "�Q�[����ʁ@�X�e�[�W%d", map.stage);
					DrawFormatString(20, 820, GetColor(255, 255, 255), "L�X�e�B�b�N:�ړ�");
					DrawFormatString(20, 840, GetColor(255, 255, 255), "RT:�_�b�V��");
					DrawFormatString(20, 860, GetColor(255, 255, 255), "LT������:�O���C�_�[�W�J");
					DrawFormatString(20, 880, GetColor(255, 255, 255), "A:�W�����v");
					DrawFormatString(20, 900, GetColor(255, 255, 255), "���[�U�[�u���b�N�̋߂��� B:���[�h�ؑ� �N��/����");
					DrawFormatString(20, 920, GetColor(255, 255, 255), "�p�x�ύX���[�h�� R�X�e�B�b�N�F���[�U�[�p�x�ύX");
					DrawFormatString(20, 940, GetColor(255, 255, 255), "Y:�|�[�Y");
					DrawFormatString(1000, 20, GetColor(255, 255, 255), "LX %d\nLY %d", pad.ThumbLX, pad.ThumbLY);
					DrawFormatString(1000, 60, GetColor(255, 255, 255), "RX %d\nRY %d", pad.ThumbRX, pad.ThumbRY);
					DrawFormatString(1000, 100, GetColor(255, 255, 255), "%lf", redLaser->theta);
					if (player->isMove == 1) {
						DrawFormatString(1250, 920, GetColor(255, 255, 255), "���̃��[�h �ړ�");
					}
					else {
						DrawFormatString(1250, 920, GetColor(255, 255, 255), "���̃��[�h �p�x�ύX");
						DrawFormatString(20, 920, GetColor(255, 0, 0), "�p�x�ύX���[�h�� R�X�e�B�b�N�F���[�U�[�p�x�ύX");
					}
					if (player->isJump == 1) {
						DrawFormatString(20, 880, GetColor(255, 0, 0), "A:�W�����v");
					}
					if (pad.ThumbLX > 4500 || pad.ThumbLX < -4500) {
						DrawFormatString(20, 820, GetColor(255, 0, 0), "L�X�e�B�b�N:�ړ�");
					}
					if (pad.LeftTrigger > 150) {
						DrawFormatString(20, 860, GetColor(255, 0, 0), "LT������:�O���C�_�[�W�J");
					}
					if (pad.RightTrigger > 150) {
						DrawFormatString(20, 840, GetColor(255, 0, 0), "RT:�_�b�V��");
					}

					break;

				case 3://�|�[�Y���-------------------------------------------------------------
					////���ŃQ�[����ʂ�`��////
					DrawGraph(0, 0, backHandle, true);
					redLaser->Draw();
					greenLaser->Draw();
					blueLaser->Draw();
					Map();
					player->Draw();
					
					GetDrawScreenGraph(0, 0, WIN_WIDTH, WIN_HEIGHT, blurHandle);
					GraphFilter(blurHandle, DX_GRAPH_FILTER_GAUSS, 32, 1200);
					DrawGraph(0, 0, blurHandle, true);
					/////-----------------////
					ui->drawColorPalette(1200, 60, colorPalette);
					ui->PauseDraw();

					if (ui->isChange == true) {
						ui->DrawFadeInOut();
					}

					DrawFormatString(1000, 20, GetColor(255, 255, 255), "LX %d\nLY %d", pad.ThumbLX, pad.ThumbLY);
					DrawFormatString(1000, 60, GetColor(255, 255, 255), "RX %d\nRY %d", pad.ThumbRX, pad.ThumbRY);
					DrawFormatString(1000, 100, GetColor(255, 255, 255), "%lf", redLaser->theta);
					break;

				case 4:

					DrawFormatString(700, 480, GetColor(255, 255, 255), "GameOver\nB�Ń��g���C");
		}
		/*DrawFormatString(1000, 500, GetColor(255, 255, 255), "state %d", player->state);
		DrawFormatString(1000, 520, GetColor(255, 255, 255), "a %d", player->idleTimer);
		DrawFormatString(700, 480, GetColor(255, 255, 255), "scene %d",scene);
		DrawFormatString(500, 550, GetColor(255, 255, 255), "fade %d", ui->fade);
		DrawFormatString(500, 570, GetColor(255, 255, 255), "re %d", ui->returnTitle);
		DrawFormatString(500, 590, GetColor(255, 255, 255), "gDraw3 %d", greenLaser->isDraw[3]);
		DrawFormatString(500, 610, GetColor(255, 255, 255), "gDraw2 %d", greenLaser->isDraw[2]);*/
		//---------  �����܂łɃv���O�������L�q  ---------//
		// (�_�u���o�b�t�@)����
		ScreenFlip();

		// 20�~���b�ҋ@(�^��60FPS)
		WaitTimer(20);

		// Windows�V�X�e�����炭�������������
		if (ProcessMessage() == -1)
			{
			break;
		}

		// ESC�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			{
			break;
		}
	}
	// Dx���C�u�����I������
	DxLib_End();

	// ����I��
	return 0;
	}