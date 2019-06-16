#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
class Land :public Layer
{
public:
	CREATE_FUNC(Land);
	virtual bool init();
	static Scene* CreateScene();
	void NameFieldEvent(Ref *pSender, cocos2d::ui::TextField::EventType type);
	void IpFieldEvent(Ref *pSender, cocos2d::ui::TextField::EventType type);
	std::string name;
	std::string IP;
};

