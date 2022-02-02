#include "DxLib.h"
#include "Player.h"
#include "Laser.h"
#include "Map.h"
#include"Colision.h"
#include "UI.h"
#include "Sturct.h"
#include "Tutorial.h"
#include<math.h>
#include "SE.h"


double easeOutCirc(double x);

void BgColor();

class Fps {
	int mStartTime;         //����J�n����
	int mCount;             //�J�E���^
	float mFps;             //fps
	static const int N = 45;//���ς����T���v����
	static const int FPS = 45;    //�ݒ肵��FPS

public:
	Fps() {
		mStartTime = 0;
		mCount = 0;
		mFps = 0;
		}

	bool Update() {
		if (mCount == 0) { //1�t���[���ڂȂ玞�����L��
			mStartTime = GetNowCount();
			}
		if (mCount == N) { //60�t���[���ڂȂ畽�ς��v�Z����
			int t = GetNowCount();
			mFps = 1000.f / ((t - mStartTime) / (float)N);
			mCount = 0;
			mStartTime = t;
			}
		mCount++;
		return true;
		}

	void Wait() {
		int tookTime = GetNowCount() - mStartTime;    //������������
		int waitTime = mCount * 1000 / FPS - tookTime;    //�҂ׂ�����
		if (waitTime > 0) {
			WaitTimer(waitTime);    //�ҋ@
			}
		}
	};



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
	Fps fps;
	// (�_�u���o�b�t�@)�`���O���t�B�b�N�̈�͗��ʂ��w��
	SetDrawScreen(DX_SCREEN_BACK);

	// �摜�Ȃǂ̃��\�[�X�f�[�^�̕ϐ��錾�Ɠǂݍ���
	BgColor();

	int titlePic = LoadGraph("Resources/title_screen.png");
	int titleCrystal = LoadGraph("Resources/title_screen-crystal.png");
	int titleTextPic[4];
	LoadDivGraph("Resources/title_screen-text.png", 4, 4, 1, 1405, 960, titleTextPic, true);

	LoadDivGraph("Resources/block.png", 16, 4, 4, 32, 32, map.chipBlock, true);
	LoadDivGraph("Resources/crystal.png", 4, 4, 1, 192, 256, map.crystalPic, true);
	int colorPalette[3];
	LoadDivGraph("Resources/color_pallette.png", 3, 3, 1, 64, 64, colorPalette, true);
	int vignette = LoadGraph("Resources/vignette.png");
	int gauss = MakeScreen(WIN_WIDTH, WIN_HEIGHT, true);


	// �Q�[�����[�v�Ŏg���ϐ��̐錾
	int scene = 0;
	int isClear = 0;
	int reset = 0;
	int isSceneChange = 0;

	int titleAlpha = 0;
	int titleTextAlpha = 0;
	int titleSelect = 0;

	int stage = 0;
	int stageMax = 4;

	int time = 0;
	int ms = 0;
	int second = 0;
	int minute = 0;

	int rAlpha = 0, gAlpha = 0, bAlpha = 0;
	//-----------------------------------------//


	//-----�X�e�[�W�I��
	int stageSelectPic[5];
	LoadDivGraph("Resources/stage_select.png", 5, 5, 1, 1408, 960, stageSelectPic, true);
	int isChange = 0;

	//-----�N���A------
	int light = LoadGraph("Resources/light.png", true);
	int clearPic[3];
	LoadDivGraph("Resources/stageclear.png", 3, 3, 1, 800, 510, clearPic, true);
	int clearPicNum = 0;
	float ex = 0;
	int count = 0;
	int clearAlpha = 255, clearAlpha2 = 0;
	double flame = 0, flameMax = 65;
	double flamePerFlamemax;

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
	//--------------�`���[�g���A���C���X�^���X����------//
	Tutorial* tutorial = new Tutorial;

	// �ŐV�̃L�[�{�[�h���p
	char keys[256] = { 0 };

	// 1���[�v(�t���[��)�O�̃L�[�{�[�h���
	char oldkeys[256] = { 0 };

	// �Q�[�����[�v
	while (1)
		{
		fps.Update();    //�X�V
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


		double dead = atan2(pad.ThumbLY, pad.ThumbLX);
		int deadX = cos(dead) * 25000;
		int deadY = sin(dead) * -25000;

		//�V�[��
		switch (scene) {
				case 0://�^�C�g�����---------------------------------------------------
					if (CheckSoundMem(se.mainBGM) == 0) {
						PlaySoundMem(se.mainBGM, DX_PLAYTYPE_LOOP, true);
					}


					if (ui->isChange == false) {
						ui->returnTitle = false;
						}
					//----�A���t�@���o----//
					titleAlpha += 3;
					if (titleAlpha >= 180) {
						titleTextAlpha += 10;
						}

					if (titleAlpha >= 255) {
						titleAlpha = 255;
						}
					if (titleTextAlpha >= 255) {
						titleTextAlpha = 255;
						}
					//------------------//

					//�X�e�[�W�I����
					if (titleTextAlpha == 255) {
						if (pad.ThumbLY > 9000 && padOld.ThumbLY < 9000) {

							if (titleSelect != 0) {
								//PlaySoundMem(menuSelectSE, DX_PLAYTYPE_BACK, true);
								titleSelect--;
								}
							}
						else if (pad.ThumbLY < -9000 && padOld.ThumbLY>-9000) {
							if (titleSelect != 3) {
								//PlaySoundMem(menuSelectSE, DX_PLAYTYPE_BACK, true);
								titleSelect++;
								}
							}
						}

					if (pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0) {
						//PlaySoundMem(menuPickSE, DX_PLAYTYPE_BACK, true);
						//�A���t�@�X�L�b�v
						if (titleTextAlpha <= 255) {
							titleAlpha = 255;
							titleTextAlpha = 255;
							}

						if (titleSelect == 3) {
							ui->isContorolMethod = true;	//������@��ʂ��J��
							}

						if (ui->isChange == false && titleSelect != 0 && titleSelect != 3) {
							ui->isChange = true;
							ui->isIn = true;
							}
						}

					//Y���������瑀����@���
					if (pad.Buttons[XINPUT_BUTTON_Y] == 1 && padOld.Buttons[XINPUT_BUTTON_Y] == 0 &&
						ui->isContorolMethod == true) {

						ui->isContorolMethod = false;
						}

					//������@��ʂ��J���Ă���
					if (ui->isContorolMethod == true) {
						ui->ContorolMagnification += 0.2;

						if (ui->ContorolMagnification > 1.4) {
							ui->ContorolMagnification = 1.4;
							}
						}
					//������@��ʂ����
					else if (ui->isContorolMethod == false) {
						ui->ContorolMagnification -= 0.2;

						if (ui->ContorolMagnification < 0.0) {
							ui->ContorolMagnification = 0.0;
							}
						}

					if (ui->isChange == true) {
						ui->SceneTransition();
						}

					if (ui->isIn == false && ui->isChange == true && ui->returnTitle == false) {
						if (titleSelect == 1) {
							//StopSoundMem(titleBGM);
							map.stage = 0;
							scene = 2;
							reset = 1;
							}
						else if (titleSelect == 2) {
							scene = 1;
							}
						}

					break;

				case 1://�X�e�[�W�I�����-----------------------------------------------
					/*if (CheckSoundMem(titleBGM) == 0) {
						PlaySoundMem(titleBGM, DX_PLAYTYPE_LOOP, true);
					}*/

					if (ui->isChange == true) {
						ui->SceneTransition();
					}

					//�X�e�[�W�I��
					if (abs(pad.ThumbLX) > abs(deadX) && abs(pad.ThumbLY) > abs(deadY)) {
						if (isChange == 0) {
							if (map.stage == 0) {
								if (pad.ThumbLX > 25000 && padOld.ThumbLX<25000 && pad.ThumbLY > -10000 && padOld.ThumbLY < 10000) {
									map.stage = 1;
									isChange = 1;
									}
								else if (pad.ThumbLY< -25000 && padOld.ThumbLY > -25000 && pad.ThumbLX > -10000 && padOld.ThumbLX < 10000) {
									map.stage = 2;
									isChange = 1;
									}
								else if (dead < -0.5 && dead > -1) {
									map.stage = 4;
									isChange = 1;
									}
								}
							else if (map.stage == 1) {
								if (pad.ThumbLX < -25000 && padOld.ThumbLX > -25000 && pad.ThumbLY > -10000 && padOld.ThumbLY < 10000) {
									map.stage = 0;
									isChange = 1;
									}
								else if (pad.ThumbLY<-20000 && padOld.ThumbLY > -25000 && pad.ThumbLX > -10000 && padOld.ThumbLX < 10000) {
									map.stage = 3;
									isChange = 1;
									}
								else if (dead < -2 && dead > -2.5) {
									map.stage = 4;
									isChange = 1;
									}
								}
							else if (map.stage == 2) {
								if (pad.ThumbLX > 25000 && padOld.ThumbLX < 25000 && pad.ThumbLY > -10000 && padOld.ThumbLY < 10000) {
									map.stage = 3;
									isChange = 1;
									}
								else if (pad.ThumbLY > 25000 && padOld.ThumbLY < 25000 && pad.ThumbLX > -10000 && padOld.ThumbLX < 10000) {
									map.stage = 0;
									isChange = 1;
									}
								else if (dead < 1 && dead > 0.5) {
									map.stage = 4;
									isChange = 1;
									}
								}
							else if (map.stage == 3) {
								if (pad.ThumbLX < -25000 && padOld.ThumbLX > -25000 && pad.ThumbLY > -10000 && padOld.ThumbLY < 10000) {
									map.stage = 2;
									isChange = 1;
									}
								else if (pad.ThumbLY > 25000 && padOld.ThumbLY < 25000 && pad.ThumbLX > -10000 && padOld.ThumbLX < 10000) {
									map.stage = 1;
									isChange = 1;
									}
								else if (dead < 2.5 && dead > 2) {
									map.stage = 4;
									isChange = 1;
									}
								}
							else if (map.stage == 4) {
								if (dead < 3.1 && dead > 1.5) {
									map.stage = 0;
									isChange = 1;
									}
								else if (dead < 1.5 && dead > 0) {
									map.stage = 1;
									isChange = 1;
									}
								else if (dead < -1.5 && dead > -3.1) {
									map.stage = 2;
									isChange = 1;
									}
								else if (dead < 0 && dead > -1.5) {
									map.stage = 3;
									isChange = 1;
									}
								}
							}
						}
					else {
						isChange = 0;
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

					tutorial->Update(pad, padOld, scene);

					if (ui->fade >= 225) {
						//StopSoundMem(titleBGM);
						scene = 2;
						reset = 1;
					}

					break;

				case 2://�Q�[�����------------------------------------------------------
					/*if (CheckSoundMem(stageBGM) == 0) {
						PlaySoundMem(stageBGM, DX_PLAYTYPE_LOOP, true);
					}*/

					if (ui->isChange == true) {
						ui->SceneTransition();
						}

					if (tutorial->isGameTutorial == true) {
						tutorial->Update(pad, padOld, scene);
						}

					if (map.stage != 0 && map.stage != 1) {
						tutorial->isGameTutorial = false;
						}

					if (isClear == 0 && tutorial->isGameTutorial == false) {

						if (map.isDraw[RED] == true && map.isDraw[GREEN] == true) {

							if (player->isMove == 0 && tutorial->Page == 4) {
								tutorial->Count = 0;
								tutorial->isGameTutorial = true;
								}
							}

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
							//StopSoundMem(stageBGM);
							isClear = 1;
						}
					}

					//---------�N���A��----------
					else if (isClear == 1) {
						if (map.stage == stageMax) {
							clearPicNum = 2;
							}
						//�N���A���o
						clearAlpha -= 2;
						if (clearAlpha < 100) {
							if (clearAlpha2 <= 216) {
								clearAlpha2 += 4;
								}
							}
						if (count == 0) {
							if (map.alpha[CYAN] > 55 && map.alpha[MAGENTA] > 55 && map.alpha[YELLOW] > 55) {
								//�T�C�Y�Ɠ��ߓx�ω�
								if (flame < flameMax) {
									flame++;
									}
								flamePerFlamemax = flame / flameMax;
								ex = 40 * easeOutCirc(flamePerFlamemax);
								if (clearAlpha <= 50) {
									count = 1;
									}
								}
							}

						//�N���A��I��
						else if (count == 1) {
							if (map.stage != stageMax) {
								if (pad.ThumbLY > 9000 && padOld.ThumbLY < 9000) {
									clearPicNum = 0;
									}
								if (pad.ThumbLY < -9000 && padOld.ThumbLY > -9000) {
									clearPicNum = 1;
									}
								}

							if (pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0) {
								if (clearPicNum == 0) {
									map.stage++;
									reset = 1;
									}
								else if (clearPicNum == 1 || clearPicNum == 2) {
									scene = 0;
									}
								}
							}
						}

						if (player->transform.posY >= WIN_HEIGHT) {
							ui->isChange = true;
							ui->isIn = true;

							if (ui->fade >= 245) {
								reset = 1;
							}
						}

					ui->magnification = 0;
					break;

				case 3://�|�[�Y���------------------------------------------------------

					if (ui->isChange == true) {
						ui->SceneTransition();
					}

					ui->PauseUpdate(pad, padOld, scene, reset, titleAlpha, titleTextAlpha);

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

				case 5:
					break;
			}

		//������
		if (reset == 1) {
			reset = 0;
			player->Reset();
			tutorial = new Tutorial;
			redLaser = new Laser(map, R_LASER);
			greenLaser = new Laser(map, G_LASER);
			blueLaser = new Laser(map, B_LASER);
			for (int i = 2; i < 8; i++) {
				map.isDraw[i] = 0;
				map.alpha[i] = 0;
				}
			isClear = 0;
			time = 0;
			second = 0;
			ms = 0;
			ui = new UI;
			count = 0;
			ex = 0;
			clearAlpha = 255;
			clearAlpha2 = 0;
			flame = 0;
			clearPicNum = 0;
			}

		//-------------------------------------------------�`�揈��------------------------------------------------------------------//

		//�V�[��
		switch (scene) {
				case 0://�^�C�g��

					DrawGraph(0, 0, titlePic, true);
					SetDrawBlendMode(DX_BLENDMODE_ADD, titleAlpha);
					DrawGraph(1408 / 2, 0, titleCrystal, true);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
					SetDrawBlendMode(DX_BLENDMODE_ADD, titleTextAlpha);

					if (titleSelect == 0) {
						DrawGraph(0, 0, titleTextPic[0], true);
						}
					else if (titleSelect == 1) {
						DrawGraph(0, 0, titleTextPic[1], true);
						}
					else if (titleSelect == 2) {
						DrawGraph(0, 0, titleTextPic[2], true);
						}
					else if (titleSelect == 3) {
						DrawGraph(0, 0, titleTextPic[3], true);
						}

					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

					////----������@�̉�ʕ`��----////
					DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, ui->ContorolMagnification, 0.0, ui->ControlImage, true);
					DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, ui->ContorolMagnification, 0.0, ui->xBoxImage, true);

					if (ui->isChange == true) {
						ui->DrawFadeInOut();
						}

					//�f�o�b�O�p
					DrawFormatString(0, 20, GetColor(255, 255, 255), "�^�C�g���N���X�^���A���t�@ %d", titleAlpha);
					DrawFormatString(0, 40, GetColor(255, 255, 255), "�^�C�g���e�L�X�g�A���t�@ %d", titleTextAlpha);
					DrawFormatString(500, 430, GetColor(255, 255, 255), "�^�C�g�����");
					DrawFormatString(500, 530, GetColor(255, 255, 255), "B�ŃX�e�[�W�I����");

					break;

				case 1://�X�e�[�W�I�����---------------------------------------------------
					if (ui->isChange == true) {
						ui->DrawFadeInOut();
						}
					DrawGraph(0, 0, stageSelectPic[map.stage], true);

					break;

				case 2://�Q�[�����----------------------------------------------------------

					DrawGraph(0, 0, map.bg[NONE], true);

					SetDrawBlendMode(DX_BLENDMODE_ADD, map.alpha[RED]);
					DrawGraph(0, 0, map.bg[RED], true);

					SetDrawBlendMode(DX_BLENDMODE_ADD, map.alpha[GREEN]);
					DrawGraph(0, 0, map.bg[GREEN], true);

					SetDrawBlendMode(DX_BLENDMODE_ADD, map.alpha[BLUE]);
					DrawGraph(0, 0, map.bg[BLUE], true);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

					if (clearAlpha < 140) {
						DrawGraph(0, 0, map.bg[NORMAL], true);
						}

					redLaser->Draw();
					greenLaser->Draw();
					blueLaser->Draw();
					Map();
					player->Draw();

					SetDrawBright(map.alpha[RED] - 50, map.alpha[GREEN] - 50, map.alpha[BLUE] - 50);
					DrawGraph(0, 0, vignette, true);
					SetDrawBright(255, 255, 255);

					if (isClear == 0) {
						ui->ButtonDraw(pad, padOld, player->transform);

						if (pad.Buttons[XINPUT_BUTTON_Y] == 1 && padOld.Buttons[XINPUT_BUTTON_Y] == 0) {
							GetDrawScreenGraph(0, 0, WIN_WIDTH, WIN_HEIGHT, gauss);
							DrawGraph(1265, WIN_HEIGHT - 48, ui->yButtanPic[1], true);
							}
						else if (pad.Buttons[XINPUT_BUTTON_Y] == 1 && padOld.Buttons[XINPUT_BUTTON_Y] == 1) {
							DrawGraph(1265, WIN_HEIGHT - 48, ui->yButtanPic[1], true);
							}
						else {
							DrawGraph(1265, WIN_HEIGHT - 48, ui->yButtanPic[0], true);
							}
						}

					//------�N���A-----
					else if (isClear == 1) {

						if (clearAlpha < 140) {
							GetDrawScreenGraph(0, 0, WIN_WIDTH, WIN_HEIGHT, blurHandle);
							GraphFilter(blurHandle, DX_GRAPH_FILTER_GAUSS, 32, 1200);
							DrawGraph(0, 0, blurHandle, true);
							}
						//���o
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, clearAlpha);
						DrawRotaGraph2(map.crystalPos[map.stage][0], map.crystalPos[map.stage][1], 32, 32, ex, 0, light, true);
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

						//�I��
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, clearAlpha2);
						DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, 1, 0.0, ui->pauseImage, true);
						DrawRotaGraph(WIN_WIDTH / 2, WIN_HEIGHT / 2, 0.8, 0.0, clearPic[clearPicNum], true);
						SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
						}

					if (tutorial->isGameTutorial == true) {
						tutorial->Draw();

						if (tutorial->Count == 40) {

							if (pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 0 ||
								pad.Buttons[XINPUT_BUTTON_B] == 1 && padOld.Buttons[XINPUT_BUTTON_B] == 1) {

								DrawRotaGraph(1180, 750, 1.4, 0.0, ui->bButtanPic[1], true);
								}
							else {
								DrawRotaGraph(1180, 750, 1.4, 0.0, ui->bButtanPic[0], true);
								}
							}
						}

					if (ui->isChange == true) {
						ui->DrawFadeInOut();
						}
					break;

				case 3://�|�[�Y���-------------------------------------------------------------
					////���ŃQ�[����ʂ�`��////
					DrawGraph(0, 0, map.bg[NONE], true);

					SetDrawBlendMode(DX_BLENDMODE_ADD, map.alpha[RED]);
					DrawGraph(0, 0, map.bg[RED], true);

					SetDrawBlendMode(DX_BLENDMODE_ADD, map.alpha[GREEN]);
					DrawGraph(0, 0, map.bg[GREEN], true);

					SetDrawBlendMode(DX_BLENDMODE_ADD, map.alpha[BLUE]);
					DrawGraph(0, 0, map.bg[BLUE], true);

					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
					redLaser->Draw();
					greenLaser->Draw();
					blueLaser->Draw();
					Map();
					player->Draw();
					SetDrawBright(map.alpha[RED], map.alpha[GREEN], map.alpha[BLUE]);
					DrawGraph(0, 0, vignette, true);
					SetDrawBright(255, 255, 255);

					GetDrawScreenGraph(0, 0, WIN_WIDTH, WIN_HEIGHT, blurHandle);
					GraphFilter(blurHandle, DX_GRAPH_FILTER_GAUSS, 32, 1200);
					DrawGraph(0, 0, blurHandle, true);
					/////-----------------////
					ui->drawColorPalette(1275, 40, colorPalette);
					ui->PauseDraw();

					if (pad.Buttons[XINPUT_BUTTON_Y] == 1 && padOld.Buttons[XINPUT_BUTTON_Y] == 0 ||
						pad.Buttons[XINPUT_BUTTON_Y] == 1 && padOld.Buttons[XINPUT_BUTTON_Y] == 1) {

						DrawGraph(1265, WIN_HEIGHT - 48, ui->yButtanPic[3], true);
						}
					else {
						DrawGraph(1265, WIN_HEIGHT - 48, ui->yButtanPic[2], true);
						}


					if (ui->isChange == true) {
						ui->DrawFadeInOut();
						}

					DrawFormatString(1000, 20, GetColor(255, 255, 255), "LX %d\nLY %d", pad.ThumbLX, pad.ThumbLY);
					DrawFormatString(1000, 60, GetColor(255, 255, 255), "RX %d\nRY %d", pad.ThumbRX, pad.ThumbRY);
					DrawFormatString(1000, 100, GetColor(255, 255, 255), "%lf", redLaser->theta);
					break;

				case 4:

					DrawFormatString(700, 480, GetColor(255, 255, 255), "GameOver\nB�Ń��g���C");

					break;
			}
		//---------  �����܂łɃv���O�������L�q  ---------//
		ScreenFlip();
		fps.Wait();        //�ҋ@

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

void BgColor() {
	int siHandle;
	int w, h;
	int r, g, b, a, gray;

	map.bg[1] = LoadGraph("Resources/background.png");
	siHandle = LoadSoftImage("Resources/background.png");
	//�摜�T�C�Y�擾
	GetSoftImageSize(siHandle, &w, &h);

	//�摜�̃O���[�X�P�[����
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			//�s�N�Z���P�ʂ̐F�擾
			GetPixelSoftImage(siHandle, x, y, &r, &g, &b, &a);
			//�F�̕��ϒl�����߂�
			gray = (r + g + b) / 3;
			//���߂����ϒl�ɒu��������
			DrawPixelSoftImage(siHandle, x, y, gray, gray, gray, a);
			}
		}
	//�O���t�n���h���ɂ���
	map.bg[NONE] = CreateGraphFromSoftImage(siHandle);
	DeleteGraph(siHandle);

	//--------------r�̂�-----------
	siHandle = LoadSoftImage("Resources/background.png");
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			//�s�N�Z���P�ʂ̐F�擾
			GetPixelSoftImage(siHandle, x, y, &r, &g, &b, &a);
			//�F�̕��ϒl�����߂�
			gray = (r + g + b) / 3;

			//���ϒl��肻�̐F�̒l���傫����Βu��������
			if (r > gray) {
				DrawPixelSoftImage(siHandle, x, y, r - gray, 0, 0, a);
				}
			else {
				DrawPixelSoftImage(siHandle, x, y, 0, 0, 0, a);
				}
			}
		}
	map.bg[RED] = CreateGraphFromSoftImage(siHandle);
	DeleteGraph(siHandle);

	//-------------g�̂�------------
	siHandle = LoadSoftImage("Resources/background.png");
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			//�s�N�Z���P�ʂ̐F�擾
			GetPixelSoftImage(siHandle, x, y, &r, &g, &b, &a);
			//�F�̕��ϒl�����߂�
			gray = (r + g + b) / 3;

			//���ϒl��肻�̐F�̒l���傫����Βu��������
			if (g > gray) {
				DrawPixelSoftImage(siHandle, x, y, 0, g - gray, 0, a);
				}
			else {
				DrawPixelSoftImage(siHandle, x, y, 0, 0, 0, a);
				}
			}
		}
	map.bg[GREEN] = CreateGraphFromSoftImage(siHandle);
	DeleteGraph(siHandle);

	//------------b�̂�------------
	siHandle = LoadSoftImage("Resources/background.png");
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			//�s�N�Z���P�ʂ̐F�擾
			GetPixelSoftImage(siHandle, x, y, &r, &g, &b, &a);
			//�F�̕��ϒl�����߂�
			gray = (r + g + b) / 3;

			//���ϒl��肻�̐F�̒l���傫����Βu��������
			if (b > gray) {
				DrawPixelSoftImage(siHandle, x, y, 0, 0, b - gray, a);
				}
			else {
				DrawPixelSoftImage(siHandle, x, y, 0, 0, 0, a);
				}
			}
		}
	map.bg[BLUE] = CreateGraphFromSoftImage(siHandle);
	DeleteGraph(siHandle);
	}


double easeOutCirc(double x) {
	return pow(x, 4);
	}