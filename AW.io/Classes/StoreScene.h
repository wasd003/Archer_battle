#pragma once
#include "cocos2d.h"
USING_NS_CC;
class StoreScene :public Layer
{
public:
	CREATE_FUNC(StoreScene);
	bool init();
	static Scene* CreateScene();
};