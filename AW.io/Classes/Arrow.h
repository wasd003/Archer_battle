#pragma once
#include "cocos2d.h"
USING_NS_CC;
class Arrow :public Sprite
{
public:
	int arrow_attack;
	double dir;
	int speed;
	void set();
	static Arrow* create(const std::string &filename);
};