#pragma once
#include "cocos2d.h"
USING_NS_CC;
class Arrow :public Sprite
{
public:
	int arrow_attack;
	int arrow_size;//箭的尺寸，用于人与箭的碰撞检测
	double dir;
	int speed;
	static Arrow*CreateArrow(const std::string &filename);
	void InitArrow(const std::string &filename);
	CREATE_FUNC(Arrow);
};