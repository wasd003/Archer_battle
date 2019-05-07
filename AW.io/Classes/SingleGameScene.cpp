#include "SingleGameScene.h"
#include "HelloWorldScene.h"
#include "TDpoint.h"
#include "Person.h"
#include "Arrow.h"
#include "Rocker.h"
USING_NS_CC;
bool SingleGameScene::init()
{
	if (!Layer::init())return false;

	//初始化全局属性
	auto winsize = Director::getInstance()->getWinSize();
	this->rockerBG_Position = Vec2(100, 100);
	this->current_point = rockerBG_Position;
	height = 50;
	//加载地图
	this->map = TMXTiledMap::create("Tank2.tmx");
	map->setTag(MapTag);
	//map->setPosition(Vec2(winsize.width / 2, winsize.height / 2));
	this->addChild(map,-1);
	//加载所有对象
	InitAllPoint(map); 
	//加载障碍层
	stop = map->getLayer("stop");
	background = map->getLayer("background");
	//在屏幕正中央创建主角
	Person* model = Person::create("person.png");
	model->setTag(ModelTag);
	this->addChild(model);
	model->setPosition(Vec2(winsize.width / 2, winsize.height / 2));

	//设置摇杆
	auto rocker = Rocker::create("CloseSelected.png","rr.png", rockerBG_Position);
	rocker->setTag(RockerTag);
	this->addChild(rocker);
	rocker->StartRocker();

	//控制人物移动
	auto MoveListen = EventListenerTouchOneByOne::create();
	MoveListen->setSwallowTouches(true);
	MoveListen->onTouchBegan = CC_CALLBACK_2(SingleGameScene::MoveBegan, this);
	MoveListen->onTouchMoved = CC_CALLBACK_2(SingleGameScene::MoveMoved, this);
	MoveListen->onTouchEnded = CC_CALLBACK_2(SingleGameScene::MoveEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(MoveListen, this);
	this->schedule(schedule_selector(SingleGameScene::MovePerson, this));


	//控制射箭
	auto ArrowListen= EventListenerTouchOneByOne::create();
	ArrowListen->onTouchBegan = CC_CALLBACK_2(SingleGameScene::ArrowBegan, this);
	ArrowListen->onTouchMoved = CC_CALLBACK_2(SingleGameScene::ArrowMoved, this);
	ArrowListen->onTouchEnded = CC_CALLBACK_2(SingleGameScene::ArrowEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(ArrowListen, this);
	this->schedule(schedule_selector(SingleGameScene::MoveArrow, this));

	
	return true;
}

Vec2 SingleGameScene::exchange(Vec2 pos)
{
	Vec2 res;
	Vec2 anchor = map->getPosition();//得到的是左下角的坐标 woc。不是中心点的坐标
	int anchor_y = map->getMapSize().height ;
	int anchor_x = 0;
	int dir_x = (pos.x - anchor.x)/map->getTileSize().width;
	int dir_y = (anchor.y - pos.y)/map->getTileSize().height;
	res.y = anchor_y + dir_y;
	res.x = anchor_x + dir_x;
	return res;
}
bool SingleGameScene::check(Vec2 pos)
{
	Vec2 tile_pos = exchange(pos);
	int gid = stop->getTileGIDAt(tile_pos);
	return gid != is_stop;
}

bool SingleGameScene::MoveBegan(Touch* t, Event* e)//人物移动
{
	auto rocker = this->getChildByTag(RockerTag);
	Sprite* rockerBG = static_cast<Sprite*>(rocker->getChildByTag(tag_rockerBG));
	Vec2 point = t->getLocation();
	if(rockerBG->getBoundingBox().containsPoint(point))
	return true;
	else return false;
}
void SingleGameScene::MoveMoved(Touch* t, Event* e)
{
	Vec2 point = t->getLocation();
	this->current_point = point;
	return;
}
void SingleGameScene::MoveEnded(Touch* t, Event *e)
{
	this->current_point = rockerBG_Position;
	return;
}
void SingleGameScene::MovePerson(float t)
{
	Person* model = static_cast<Person*>(this->getChildByTag(ModelTag));
	float dir = Rocker::getRad(rockerBG_Position, current_point);
	if (rockerBG_Position == current_point)return;
	float dx = model->speed * cos(dir);
	float dy = model->speed * sin(dir);
	Vec2 next_pos = Vec2(model->getPositionX() +height*(1+cos(dir))+ dx, model->getPositionY() +height*(1+sin(dir))+ dy);//人物下一刻即将移动到的位置  将人物视为一个圆柱体
	Vec2 next_pos_right= Vec2(model->getPositionX() + height * 2 + dx, model->getPositionY() + height ); 
	if (dx < 0)
	{
		next_pos_right = Vec2(model->getPositionX()  + dx, model->getPositionY() + height);
	}
	Vec2 next_pos_up= Vec2(model->getPositionX() + height, model->getPositionY() + height * 2 + dy);
	if (dy < 0)
	{
		next_pos_up = Vec2(model->getPositionX() + height, model->getPositionY()  + dy);
	}
	if (check(next_pos))//碰撞检测
	{
		map->setPosition(Vec2(map->getPositionX() - dx, map->getPositionY() - dy));
	}
	else if(check(next_pos_right))
	{
		map->setPositionX(map->getPositionX() - dx);
	}
	else if (check(next_pos_up))
	{
		map->setPositionY(map->getPositionY() - dy);
	}
}

bool SingleGameScene::ArrowBegan(Touch* t, Event *e)
{
	
	auto rocker = this->getChildByTag(RockerTag);
	Sprite* rockerBG = static_cast<Sprite*>(rocker->getChildByTag(tag_rockerBG));
	Vec2 point = t->getLocation();
	if (!rockerBG->getBoundingBox().containsPoint(point))
	{
		Vec2 point = t->getLocation();
		start = point;
		return true;
	}
		
	else return false;
}
void SingleGameScene::ArrowMoved(Touch* t, Event*e)//射箭
{
	
	
	//待添加：：瞄准时的瞄准线
		
}
void SingleGameScene::ArrowEnded(Touch*t, Event*e)
{

	Person* model = static_cast<Person*>(this->getChildByTag(ModelTag));
	auto arrow = Arrow::CreateArrow("CloseNormal.png");
	arrow->setPosition(model->getPosition());
	float dir = Rocker::getRad(start, t->getLocation());
	arrow->dir = dir;
	AllArrow.pushBack(arrow);
}
void SingleGameScene::MoveArrow(float t)
{
	Vector<Arrow*>ToErase;
	for (auto nowArrow : AllArrow)
	{
		
		if (!se.count(nowArrow))//添加箭
		{
			this->addChild(nowArrow);
			se.insert(nowArrow);
		}
		float dx = nowArrow->speed*cos(nowArrow->dir);//移动箭
		float dy = nowArrow->speed*sin(nowArrow->dir);
		nowArrow->setPosition(Vec2(nowArrow->getPositionX() + dx, nowArrow->getPositionY() + dy));
		
		if (!check(nowArrow->getPosition()))//箭与墙壁的碰撞检测
		{
			this->removeChild(nowArrow);
			ToErase.pushBack(nowArrow);
		}
	}
	for (auto x : ToErase)
	{
		AllArrow.eraseObject(x);
		se.erase(x);
	}
}
Scene* SingleGameScene::CreateScene()
{
	auto scene = Scene::create();
	auto layer = SingleGameScene::create();
	scene->addChild(layer);
	return scene;
}
void SingleGameScene::InitAllPoint(TMXTiledMap*map)
{
	ValueVector value;
	value = map->getObjectGroup("obj")->getObjects();//得到对象层数据
	for (int i = 0; i < value.size(); i++)
	{
		ValueMap val = value.at(i).asValueMap();//获取第i个点
		TDpoint *newp = TDpoint::createPoint(val.at("x").asInt(), val.at("y").asInt());
		allpoint.pushBack(newp);
	}
}

