#pragma once
class Animation {
public:
	int playerFrame;
	int playerTimer;
	int bButtonFrame;
	int bButtonTimer;
	int rStickFrame;
	int rStickTimer;

	Animation();
	void animation(int& frame, int& timer, int frameCount, int maxGraph);
	};

