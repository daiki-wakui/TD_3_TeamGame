#pragma once
#include "DxLib.h"

class Tutorial {
public:

	int Count;
	int Page;
	int isGameTutorial;
	int framePic;
	int fontPic[6];
	Tutorial();

	void Update(XINPUT_STATE pad, XINPUT_STATE padOld, int& scene);
	void Draw();
	};

