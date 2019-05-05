#pragma once
#include "cocos2d.h"
USING_NS_CC;
class HelpScene :public Layer
{
public:
	CREATE_FUNC(HelpScene);
	bool init();
	static Scene* CreateScene();
};