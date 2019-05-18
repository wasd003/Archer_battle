#pragma once
#include "cocos2d.h"
#include "Person.h"
USING_NS_CC;
class Person;
class Arrow :public Sprite
{
public:
	int arrow_attack;
	int arrow_size;//箭的尺寸，用于人与箭的碰撞检测
	double dir;
	int speed;
	int range;//射程
	Vec2 StartPosition;//箭的起始位
	Person* master;//箭的主人
	static Arrow*CreateArrow(const std::string &filename);
	void InitArrow(const std::string &filename);
	std::string picture;
	CREATE_FUNC(Arrow);
};