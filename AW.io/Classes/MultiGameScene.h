#pragma once
#include "cocos2d.h"
#include "cocos2d\cocos\network\HttpClient.h" 
#include "cocos2d\cocos\network\HttpRequest.h"
#include "cocos2d\cocos\network\HttpResponse.h"
using namespace cocos2d::network;
USING_NS_CC;
class MultiGameScene :public Layer
{
public:
	CREATE_FUNC(MultiGameScene);
	bool init();
	static Scene* CreateScene();
private:
	void Get(float t);
	void Post(float t);
	void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
};
