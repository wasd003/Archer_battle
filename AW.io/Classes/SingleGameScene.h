#pragma once
#include "cocos2d.h"
#include "TDpoint.h"
#include "Arrow.h"
#include "Person.h"
#include<vector>
USING_NS_CC;
class SingleGameScene :public Layer
{
public:
	//初始化系列函数
	CREATE_FUNC(SingleGameScene);
	bool init();
	static Scene* CreateScene();
	void InitAllPoint(TMXTiledMap*map);


	//标签系列
	static const int MapTag = 88;//地图的标签
	static const int LayerTag = 120;//背景层的标签
	static const int RockerTag = 66;//摇杆标签
	static const int ModelTag = 99;//主角标签
	static const int tag_rocker = 50;//遥杆标签
	static const int tag_rockerBG = 60;//摇杆背景标签
	static const int is_stop = 15;//障碍物图块的GID

	//储存容器
	Vector<TDpoint*> allpoint;//保存所有点
	Vector<Arrow*>AllArrow;//保存所有箭
	Vector<Person*>AllPerson;//保存所有人
	std::set<Arrow*>se;//用来防止重复addchild

	//全局属性
	float R;//摇杆半径
	Vec2 current_point;//当前触摸点  用来辅助人物移动
	Vec2 rockerBG_Position;//摇杆中心点的坐标
	Vec2 start;//射箭时初始的触摸坐标
	TMXTiledMap* map;//地图
	TMXLayer* stop; //障碍层


	//触摸回调函数系列
	bool MoveBegan(Touch* t, Event* e);//人物移动
	void MoveMoved(Touch* t, Event* e);
	void MoveEnded(Touch* t, Event* e);
	void MovePerson(float t);

	bool ArrowBegan(Touch* t, Event *e);//射箭
	void ArrowMoved(Touch* t, Event*e);
	void ArrowEnded(Touch* t, Event*e);
	void MoveArrow(float t);
	
	
	//功能函数
	Vec2 exchange(Vec2 pos);

	//临时（debug用）
	std::vector<int>AllGid;
	struct p
	{
		std::pair<int, int>xy;
		int gid;
		p(std::pair<int, int> xy, int gid) :xy(xy), gid(gid) {}
	};
	std::vector<p>movement;
};