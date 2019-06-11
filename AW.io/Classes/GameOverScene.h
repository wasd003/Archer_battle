#pragma once
#include "cocos2d.h"
USING_NS_CC;
class GameOverScene:public Layer
{
public:
	CREATE_FUNC(GameOverScene);
	virtual bool init();
	static Scene* CreateScene();
};