#pragma once
#include "cocos2d.h"
#include "Arrow.h"
#include <string>
USING_NS_CC;
class Person :public Sprite
{
public:
	static Person* create(const std::string& filename);//创建精灵
	void set();//为人物的各类属性赋值
	int max_blood;//血量上限
	int blood;//当前血量
	int blue;//当前蓝量
	float attack;//攻击力放大倍数
	float defence;//防御力
	int speed;//移动速度
	int arrow_speed;//箭的移动速度
	Arrow weapon;
};