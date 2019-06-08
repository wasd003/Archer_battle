#pragma once
#include "cocos2d.h"
#include <string>
USING_NS_CC;

class Rocker :public Layer
{
public:
	static Rocker* create(const std::string& RockerImage, const std::string& RockerBackImage,Vec2 posBG,Vec2 pos);
	void StartRocker();
	void set();
	static const int tag_rocker = 50;//遥杆标签
	static const int tag_rockerBG = 60;//摇杆背景标签
	static float getRad(Vec2 pos1, Vec2 pos2);//获得两点之间的角度
private:
	float R;//遥杆半径
	bool onTouchBegan(Touch* t, Event* e);
	void onTouchMoved(Touch* t, Event* e);
	void onTouchEnded(Touch* t, Event* e);
};
