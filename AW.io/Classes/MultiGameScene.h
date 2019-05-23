#pragma once
#include "cocos2d.h"
#include "cocos2d\cocos\network\HttpClient.h" 
#include "cocos2d\cocos\network\HttpRequest.h"
#include "cocos2d\cocos\network\HttpResponse.h"
#include "ODsocket.h"
using namespace cocos2d::network;
USING_NS_CC;
class MultiGameScene :public Layer
{
public:
	static void* getMessage(void*);
	CREATE_FUNC(MultiGameScene);
	virtual bool init();
	static Scene* CreateScene();
	
	ODsocket* sock_client;
	std::string strmsg;
	void update(float t);

private:
	void Get(float t);
	void Post(float t);
	void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
};
