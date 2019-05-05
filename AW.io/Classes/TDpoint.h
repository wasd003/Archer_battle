#pragma once
#include "cocos2d.h"
USING_NS_CC;
class TDpoint :public Ref
{
public :
	int px;
	int py;
	static TDpoint* createPoint(int x, int y);
};