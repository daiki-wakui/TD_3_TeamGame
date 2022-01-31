#pragma once
#include"Sturct.h"
#include "Map.h"

int Colision(TransForm transform, MAP map, int x, int y, int hitSide);

int Colision(TransForm transform, int x, int y);

bool LineLine(float laserStartX, float laserStartY, float* laserEndX, float* laserEndY,
	float startX, float startY, float endX, float endY);

bool LineBox(float laserStartX, float laserStartY, float* laserEndX, float* laserEndY,
	float top, float bottom, float left, float right);

bool LineCrystal(float laserStartX, float laserStartY, float* laserEndX, float* laserEndY,
	float top, float bottom, float middle, float left, float right, float middleX);

int LineMirror(float laserStartX, float laserStartY, float* laserEndX, float* laserEndY, MAP map, int x, int y);