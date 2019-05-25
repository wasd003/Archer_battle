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
	static void* getMessage(void*);//从服务端接受信息
	void postMessage();//向服务端发送信息
	CREATE_FUNC(MultiGameScene);
	virtual bool init();
	static Scene* CreateScene();
	
	ODsocket* sock_client;//客户端套接字
	std::string strmsg;//从服务器接收到的信息
	std::string	MessageToPost;//要发送给服务器的信息
	void update(float t);

private:
	void Get(float t);
	void Post(float t);
	void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
};
