#pragma once
#include "cocos2d.h"
#include "TDpoint.h"
#include "Arrow.h"
#include "Person.h"
#include<vector>
#include<set>
USING_NS_CC;
class SingleGameScene :public Layer
{
public:
	//初始化系列函数
	CREATE_FUNC(SingleGameScene);
	bool init();
	static Scene* CreateScene();
	//void InitAllPoint(TMXTiledMap*map);
	void InitValue();//属性初始化
	void initWeapon();//武器初始化
	void InitMap();//地图初始化
	
private:


	//标签系列
	//static const int MapTag = 88;//地图的标签
	//static const int LayerTag = 120;//背景层的标签
	//static const int is_stop = 15;//障碍物图块的GID
	//static const int is_blue = 8;
	//static const int is_blood = 11;
	static const int RockerTag = 66;//摇杆标签
	static const int ModelTag = 99;//主角标签
	static const int tag_rocker = 50;//遥杆标签
	static const int tag_rockerBG = 60;//摇杆背景标签
	static const int BloodTag = 68;
	static const int MenuTag = 102;
	


	//储存容器
	//Vector<TDpoint*> allpoint;//保存所有点
	//std::set<Arrow*>se;//用来防止重复addchild
	//std::set<Vec2> se;//保存所有被吃掉的星星和心
	Vector<Arrow*>AllArrow;//保存所有箭
	Vector<Person*>AllPerson;//保存所有人
	Map<Person*, Person*> hash_table;//确定每个人的移动朝向
	Map<Person*, Sprite*> blood_hash;//显示每个怪物的血量
	Vector<Arrow*>AllWeapon;//武器数组
	Vector<Sprite*>AllStar;//所有心
	Vector<Sprite*>AllHeart;//所有星星
	std::set<Vec2> StopCheck;//障碍检测


	//属性与游戏参数
	//TMXLayer* stop; //障碍层
	//TMXLayer* background;//背景层
	//TMXLayer* star;
	//int low;//随机产生怪物的横纵坐标最小值
	//int high;//随机产生怪物的横纵坐标的最大值
	float R;//摇杆半径
	Vec2 current_point;//当前触摸点  用来辅助人物移动
	Vec2 rockerBG_Position;//摇杆中心点的坐标
	Vec2 start;//射箭时初始的触摸坐标
	TMXTiledMap* map;//地图
	int height;//从人物的中心点出发到达矩形的四条边的距离
	int MonsterNumber;//维持局面中至少的怪物个数
	int attack_area;//方圆attack_area内，如果有人，会默认朝该人移动
	int min_attack_area;//怪物之间距离允许的最小值
	Person* NullPerson;
	Arrow* NowWeapon_1;
	Arrow* NowWeapon_2;
	Arrow* NowWeapon_3;
	Vec2 One;//四个象限的地图角的坐标
	Vec2 Two;
	Vec2 Three;
	Vec2 Four;
	static const  int DY = 80;//血条距离人头顶的高度
	int score;//得分
	Label* LabelScore;
	Label* LabelMagic;

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
	Vec2 exchange(Vec2 pos);//将openGL坐标转化为瓦片地图坐标
	bool check(Vec2 pos);//判断能否移动
	inline int random(int a, int b);//随机数
	inline double distance(Vec2 pos1, Vec2 pos2);//openGL坐标距离
	void GetPos();//获得地图的四个角的坐标
	void ArrowCopy(Arrow* first, Arrow* second);//箭的深拷贝
	bool IsInBound(Vec2 pos);


	//物品
	void ShowBlood(float t);
	void ChangeWeapon(float t);//改变武器
	void MenuCallBack(cocos2d::Ref*pSender);
	void InitHeartStar();//初始化心和星星
	void CreateHeartStar(const std::string &filename);
	void SupplyHeartStar(float t);


	//添加AI
	void CreateMonster(float t);
	void MoveDirect(float t);//确定每个人移动的朝向
	void MoveAllPerson(float t);//根据朝向决定移动每一个人
	void Shoot(float t);
	void Hurt(float t);//人与箭的碰撞检测
	void Dead(float t);//人的死亡判定，资源回收
	void InitMonster();
	//debug
	int counts;
	Vector<Sprite*>debug;
	
};