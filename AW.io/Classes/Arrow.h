#pragma once
#include "cocos2d.h"
USING_NS_CC;
class Arrow :public Sprite
{
public:
	int arrow_attack;
	double dir;
	int speed;
	static Arrow*CreateArrow(const std::string &filename);
	void InitArrow(const std::string &filename);
	CREATE_FUNC(Arrow);
};