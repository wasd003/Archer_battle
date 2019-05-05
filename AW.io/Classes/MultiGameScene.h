#pragma once
#include "cocos2d.h"
USING_NS_CC;
class MultiGameScene :public Layer
{
public:
	CREATE_FUNC(MultiGameScene);
	bool init();
	static Scene* CreateScene();
};
