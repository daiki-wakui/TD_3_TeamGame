#include "Animation.h"

Animation::Animation() {
	this->playerFrame = 0;
	this->playerTimer = 0;
	this->bButtonFrame = 0;
	this->bButtonTimer = 0;
	this->rStickFrame = 0;
	this->rStickTimer = 0;

	}

//�A�j���[�V�����֐�(�t���[�����A�摜�̔ԍ��A�摜�̐؂�ւ��t���[���A�摜�̐�)
void Animation::animation(int& frame, int& timer, int frameCount, int maxGraph) {
	frame++;

	if (frame >= frameCount) {
		timer++;
		frame = 0;
		}

	if (timer >= maxGraph) {
		timer = 0;
		}
	}