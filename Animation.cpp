#include "Animation.h"

Animation::Animation() {
	this->playerFrame = 0;
	this->playerTimer = 0;
	this->bButtonFrame = 0;
	this->bButtonTimer = 0;
	this->rStickFrame = 0;
	this->rStickTimer = 0;

	}

//アニメーション関数(フレーム数、画像の番号、画像の切り替えフレーム、画像の数)
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