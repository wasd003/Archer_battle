#include "SingleGameScene.h"
#include "HelloWorldScene.h"
#include "TDpoint.h"
#include "Person.h"
#include "Arrow.h"
#include "Rocker.h"
#include <ctime>
USING_NS_CC;
inline int SingleGameScene::random(int a, int b)
{
	return (((double)rand() / RAND_MAX) * (b - a) + a);
}
bool SingleGameScene::init()
{
	if (!Layer::init())return false;

	//初始化全局属性
	auto winsize = Director::getInstance()->getWinSize();
	this->rockerBG_Position = Vec2(100, 100);
	this->current_point = rockerBG_Position;
	this->MonsterNumber = 4;
	height = 50;
	low =200, high = 800;
	attack_area = 500;
	min_attack_area =250 ;
	counts = 0;
	NullPerson = Person::CreatePerson("person.png");
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
	Person* model = Person::CreatePerson("person.png");
	model->setTag(ModelTag);
	this->addChild(model);
	model->setPosition(Vec2(winsize.width / 2, winsize.height / 2));

	//设置摇杆
	auto rocker = Rocker::create("CloseSelected.png","rr.png", rockerBG_Position);
	rocker->setTag(RockerTag);
	this->addChild(rocker);
	rocker->StartRocker();


	//实现怪物出生，移动，攻击，受击检测，死亡
	this->schedule(schedule_selector(SingleGameScene::CreateMonster, this));
	this->schedule(schedule_selector(SingleGameScene::MoveDirect, this));
	this->schedule(schedule_selector(SingleGameScene::MoveAllPerson, this));
	this->schedule(schedule_selector(SingleGameScene::Shoot), 1);
	this->schedule(schedule_selector(SingleGameScene::MoveArrow, this));
	this->schedule(schedule_selector(SingleGameScene::Hurt, this));
	this->schedule(schedule_selector(SingleGameScene::Dead, this));


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
inline double SingleGameScene::distance(Vec2 pos1, Vec2 pos2)
{
	double ans = (pos1.x - pos2.x)*(pos1.x - pos2.x) + (pos1.y - pos2.y)*(pos1.y - pos2.y);
	return sqrt(ans);
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
		for (auto NowPerson : AllPerson)
		{
			NowPerson->setPosition(Vec2(NowPerson->getPositionX() - dx, NowPerson->getPositionY() - dy));
		}
	}
	else if(check(next_pos_right))
	{
		map->setPositionX(map->getPositionX() - dx);
		for (auto NowPerson : AllPerson)
		{
			NowPerson->setPositionX(NowPerson->getPositionX() - dx);
		}
	}
	else if (check(next_pos_up))
	{
		map->setPositionY(map->getPositionY() - dy);
		for (auto NowPerson : AllPerson)
		{
			NowPerson->setPositionY(NowPerson->getPositionY() - dy);
		}
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
	this->addChild(arrow);
}
void SingleGameScene::MoveArrow(float t)
{
	Vector<Arrow*>ToErase;
	for (auto nowArrow : AllArrow)
	{
/*
		if (!se.count(nowArrow))//添加箭
		{
			this->addChild(nowArrow);
			se.insert(nowArrow);
		}
*/
		float dx = nowArrow->speed*cos(nowArrow->dir);//移动箭
		float dy = nowArrow->speed*sin(nowArrow->dir);
		Vec2 next_pos = Vec2(nowArrow->getPositionX() + dx, nowArrow->getPositionY() + dy);
		if (!check(next_pos))//箭与墙壁的碰撞检测
		{
			this->removeChild(nowArrow);
			ToErase.pushBack(nowArrow);
		}
		else
		nowArrow->setPosition(next_pos);
	}
	for (auto x : ToErase)
	{
		AllArrow.eraseObject(x);
		//se.erase(x);
	}
}


void SingleGameScene::CreateMonster(float t)
{
	srand(time(NULL));
	if (AllPerson.size() > MonsterNumber)
	{
		return;
	}
	while (AllPerson.size() < MonsterNumber)
	{
		
		auto monster = Person::CreatePerson("person.png");
		Vec2 pos;
		pos.x = random(low,high);
		pos.y = random(low, high);
		monster->setPosition(pos);//随机地生成怪物
		this->addChild(monster);
		AllPerson.pushBack(monster);
	}
}
void SingleGameScene::MoveDirect(float t)
{
	
	for (auto first:AllPerson)//怪物将选择朝与自己距离较近的
	{
		bool flag = false;
		for (auto second:AllPerson)
		{
			if (first!=second)
			{
				if (distance(first->getPosition(), second->getPosition()) <attack_area)
				{
					hash_table.insert(first,second);//找到距离小于attack_area的两个点，添加目标
					flag = true;
					break;
				}
			}
		}
		if (!flag)
		{
			hash_table.insert(first, NullPerson);
		}
	}
	
}
void SingleGameScene::Shoot(float t)
{
	for (auto NowPerson : AllPerson)
	{
		if (hash_table.at(NowPerson) != NullPerson)
		{
			auto target = hash_table.at(NowPerson);
			auto arrow = Arrow::CreateArrow("CloseNormal.png");
			arrow->setPosition(NowPerson->getPosition());
			float dir = Rocker::getRad(NowPerson->getPosition(), target->getPosition());
			arrow->dir = dir;
			AllArrow.pushBack(arrow);
			this->addChild(arrow);
		}
	}
}
void SingleGameScene::MoveAllPerson(float t)
{
	srand(time(NULL));
	int dx[4] = { -1,0,1,0 };
	int dy[4] = { 0,1,0,-1 };
	for (auto NowPerson:AllPerson)
	{
		if (hash_table.at(NowPerson)!=NullPerson)
		{
			auto direct = hash_table.at(NowPerson);
			float dir = Rocker::getRad(NowPerson->getPosition(), direct->getPosition());
			Vec2 next_pos = Vec2(NowPerson->getPositionX() + NowPerson->speed*cos(dir), NowPerson->getPositionY() + NowPerson->speed*sin(dir));
			auto dis = distance(NowPerson->getPosition(), direct->getPosition());
			if (dis < min_attack_area)
			{
				next_pos= Vec2(NowPerson->getPositionX() - NowPerson->speed*cos(dir), NowPerson->getPositionY() - NowPerson->speed*sin(dir));
			}
			if (check(next_pos))
			{
				NowPerson->setPosition(next_pos);
			}
/*
			else
			{
				if (!check(NowPerson->getPosition()))
				{
					for (int dir = 0; dir < 4; dir++)
					{
						next_pos = Vec2(NowPerson->getPositionX() + dx[dir], NowPerson->getPositionY() + dy[dir]);
						if (check(next_pos))
						{
							NowPerson->setPosition(next_pos);
							break;
						}
					}
				}
			}
*/
		}
		else//如果value是NullPerson，随机移动
		{
/*
			for (int dir = 0; dir < 4; dir++)
			{
				Vec2 next_pos = Vec2(NowPerson->getPositionX() + dx[dir], NowPerson->getPositionY() + dy[dir]);
				if (check(next_pos))
				{
					NowPerson->setPosition(next_pos);
					break;
				}
			}
*/

			
			int dir = random(0, 3);
			Vec2 next_pos = Vec2(NowPerson->getPositionX() + NowPerson->speed*dx[dir], NowPerson->getPositionY() + NowPerson->speed*dy[dir]);
			if (check(next_pos))
			{
				NowPerson->setPosition(next_pos);
			}

		}
	}
}
void SingleGameScene::Hurt(float t)
{
	Vector<Arrow*>ToErase;
	for (auto NowPerson : AllPerson)
	{
		Rect NowPerson_pos = Rect(NowPerson->getPositionX(), NowPerson->getPositionY(), height * 2, height * 2);
		for (auto NowArrow : AllArrow)
		{
			Rect NowArrow_pos = Rect(NowArrow->getPositionX(), NowArrow->getPositionY(), NowArrow->arrow_size, NowArrow->arrow_size);
			if (NowPerson_pos.intersectsRect(NowArrow_pos))//箭射中人
			{
				NowPerson->blood -= NowArrow->arrow_attack;//掉血
				//ToErase.pushBack(NowArrow);
				//NowArrow->removeFromParent();
			}
		}
	}
	for (auto x : ToErase)//删除箭
	{
		AllArrow.eraseObject(x);
		//se.erase(x);
	}
}
void SingleGameScene::Dead(float t)
{
	Vector<Person*>ToErase;
	
	for (auto NowPerson : AllPerson)
	{
		if (NowPerson->blood <= 0)
		{
			ToErase.pushBack(NowPerson);
			NowPerson->removeFromParent();
		}
	}
	for (auto x : ToErase)
	{
		AllPerson.eraseObject(x);
		hash_table.erase(hash_table.find(x));//把key是x的哈希删除
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

