#pragma once
#include "cocos2d.h"
#include "cocos2d\cocos\network\HttpClient.h" 
#include "cocos2d\cocos\network\HttpRequest.h"
#include "cocos2d\cocos\network\HttpResponse.h"
#include "ODsocket.h"
#include "Person.h"
#include "SingleGameScene.h"
#include "Arrow.h"
#include "Rocker.h"
using namespace cocos2d::network;
USING_NS_CC;
using namespace std;
class MultiGameScene :public SingleGameScene
{
public:
	
	
	CREATE_FUNC(MultiGameScene);
	virtual bool init();
	static Scene* CreateScene();
private:
	//联网系列
	void update(float t);
	//static void* getMessage(void*);//从服务端接受信息
	//static void* postMessage(void*);//向服务端发送信息
	void postMessage(float t);
	void getMessage(float t);
	ODsocket* sock_client;//客户端套接字
	std::string strmsg;//从服务器接收到的信息
	std::string	MessageToPost;//要发送给服务器的信息
	void StringToData(float t);//解析json串
	void DataToString();//将数据封装成json串，转化后的串储存在MessageToPost中

	//数据成员
	Vector<Arrow*>MyArrow;//保存我射出的箭
	Map<string, Person*> NameToPerson;//根据姓名查找玩家
	set<string>dead;//死亡名单
	//函数重写
	//void InitAllPerson();//根据服务器广播的所有人的行列坐标设置所有人的初始位置
	//virtual void MoveAllPerson(float t);//更新所有人的位置
	//virtual void Shoot(float t);//模拟其余玩家的射箭
	virtual void Dead(float t);//自己死，场景跳转，别人死，从排行榜上除名
	virtual void ArrowEnded(Touch* t, Event*e);//需要把射出的箭添加进MyArrow数组中

	//功能函数
	Vec2 ToOpenGL(Vec2 pos);//将tile坐标转化为OpenGL坐标

	//debug
	int times = 0;
private:
	void Get(float t);
	void Post(float t);
	void onHttpRequestCompleted(HttpClient *sender, HttpResponse *response);
};
