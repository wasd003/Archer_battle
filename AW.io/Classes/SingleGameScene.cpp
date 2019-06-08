#include "SingleGameScene.h"
#include "HelloWorldScene.h"
#include "TDpoint.h"
#include "Person.h"
#include "Arrow.h"
#include "Rocker.h"
#include <ctime>
#include<cstring>
#include<string>
USING_NS_CC;

bool SingleGameScene::init()
{
	if (!Layer::init())return false;

	//初始化
	auto winsize = Director::getInstance()->getWinSize();
	this->InitValue();
	this->initWeapon();
	

	//加载地图
	this->map = TMXTiledMap::create("map.tmx");
	map->setPosition(Vec2(224, -2074));
	this->addChild(map, -1);
	GetPos();
	InitMap();
	
	//加载障碍层和背景层
	/*
	stop = map->getLayer("stop");
	stop->retain();
	background = map->getLayer("background");
	background->retain();
	star = map->getLayer("star");
	star->retain();
	*/

	//创建主角
	auto model = Person::CreatePerson("person.png");
	model->setTag(ModelTag);
	this->addChild(model);
	model->setPosition(Vec2(winsize.width / 2, winsize.height / 2));
	AllPerson.pushBack(model);
	//创建怪物
	InitMonster();
	//创建星星和心
	InitHeartStar();
	//设置摇杆
	auto rocker = Rocker::create("CloseSelected.png","rr.png", rockerBG_Position,Vec2(130,130));
	rocker->setTag(RockerTag);
	this->addChild(rocker);
	rocker->StartRocker();


	//实现怪物补充，移动，攻击，受击检测，死亡
	this->schedule(schedule_selector(SingleGameScene::CreateMonster), 1);
	this->schedule(schedule_selector(SingleGameScene::MoveDirect, this));
	this->schedule(schedule_selector(SingleGameScene::MoveAllPerson, this));
	this->schedule(schedule_selector(SingleGameScene::Shoot), 3);
	this->schedule(schedule_selector(SingleGameScene::MoveArrow, this));
	this->schedule(schedule_selector(SingleGameScene::Hurt, this));
	this->schedule(schedule_selector(SingleGameScene::Dead, this));

	//星星和心的补充
	this->schedule(schedule_selector(SingleGameScene::SupplyHeartStar, this));

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


	//显示血量
	this->schedule(schedule_selector(SingleGameScene::ShowBlood, this));
	this->schedule(schedule_selector(SingleGameScene::ChangeWeapon, this));
	//this->schedule(schedule_selector(SingleGameScene::ChangeWeapon), 1);
	

	return true;
}

//功能函数
Vec2 SingleGameScene::exchange(Vec2 pos)
{

	Vec2 anchor = map->getPosition();//得到的是左下角的坐标 
	int height = 3000;
	float delta_y = height + anchor.y - pos.y;
	float delta_x = pos.x - anchor.x;
	int row = delta_y / 30;
	int col = delta_x /30;

	return Vec2(row, col);
}
bool SingleGameScene::check(Vec2 pos)
{
	Vec2 tile_pos = exchange(pos);
	return !StopCheck.count(tile_pos);
}
void SingleGameScene::GetPos()
{

	Three = map->getPosition();
	Three.x += 500, Three.y +=500;
	Two = Three + Vec2(0, 2000);
	One = Two + Vec2(2000, 2000);
	Four = Three + Vec2(2000,0);
}
inline double SingleGameScene::distance(Vec2 pos1, Vec2 pos2)
{
	double ans = (pos1.x - pos2.x)*(pos1.x - pos2.x) + (pos1.y - pos2.y)*(pos1.y - pos2.y);
	return sqrt(ans);
}
bool SingleGameScene::IsInBound(Vec2 pos)
{
	Vec2 anchor = map->getPosition();
	return (pos.x >= anchor.x + 300 && pos.x <= anchor.x + 2700 && pos.y >= anchor.y + 300 && pos.y <= anchor.y + 2700);
	
}
void SingleGameScene::ArrowCopy(Arrow* first,Arrow* second)
{
	first->speed = second->speed;
	first->range = second->range;
	first->arrow_attack = second->arrow_attack;
	first->picture = second->picture;
	first->is_attack = second->is_attack;
	first->is_range = second->is_range;
	first->is_speed = second->is_speed;
	first->is_weapon = second->is_weapon;
}
inline int SingleGameScene::random(int a, int b)
{
	return (((double)rand() / RAND_MAX) * (b - a) + a);
}



//移动模块

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
	Vector<Sprite*>ToEraseStar,	ToEraseHeart;
	float dir = Rocker::getRad(rockerBG_Position, current_point);
	if (rockerBG_Position == current_point)return;
	float dx = model->speed * cos(dir);
	float dy = model->speed * sin(dir);
	Vec2 NowPos = exchange(Vec2(model->getPositionX()+height,model->getPositionY()+height));//获得当前人物所处的tile坐标
	
	Vec2 next_pos = Vec2(model->getPositionX() +height*(cos(dir)), model->getPositionY() +height*(sin(dir)));//人物下一刻即将移动到的位置  将人物视为一个圆柱体
	for (auto NowStar : AllStar)
	{
		if (distance(NowStar->getPosition(), model->getPosition()) < 30)
		{
			ToEraseStar.pushBack(NowStar);
			model->blue += 5;
			LabelMagic->setString(String::createWithFormat("Magic:%d", model->blue)->_string);
			this->removeChild(NowStar);
		}
	}
	for (auto NowHeart : AllHeart)
	{
		if (distance(NowHeart->getPosition(), model->getPosition()) < 30)
		{
			ToEraseHeart.pushBack(NowHeart);
			model->blood += 5;
			if (model->blood > 100)model->blood = 100;
			this->removeChild(NowHeart);
		}
	}
	for (auto x : ToEraseStar)
	{
		AllStar.eraseObject(x);
	}
	for (auto x : ToEraseHeart)
	{
		AllHeart.eraseObject(x);
	}
	Vec2 next_pos_right= Vec2(model->getPositionX() + height , model->getPositionY()  ); 
	if (dx < 0)
	{
		next_pos_right = Vec2(model->getPositionX() -height, model->getPositionY() );
	}
	Vec2 next_pos_up= Vec2(model->getPositionX() , model->getPositionY() + height );
	if (dy < 0)
	{
		next_pos_up = Vec2(model->getPositionX(), model->getPositionY() -height);
	}
	Vec2 SetPos = Vec2(0, 0);
	if (check(next_pos))//碰撞检测
	{
		SetPos = Vec2(-dx,-dy);
	}
	else if(check(next_pos_right))
	{
		SetPos = Vec2(-dx, 0);
	}
	else if (check(next_pos_up))
	{
		SetPos = Vec2(0, -dy);
	}
	//地图上其他精灵跟随移动
	map->setPosition(Vec2(map->getPositionX() +SetPos.x, map->getPositionY()+ SetPos.y));
	for (auto NowPerson : AllPerson)
	{
		if (NowPerson == model)continue;
		NowPerson->setPosition(Vec2(NowPerson->getPositionX() + SetPos.x, NowPerson->getPositionY() + SetPos.y));
	}
	for (auto NowStar : AllStar)
	{
		NowStar->setPosition(Vec2(NowStar->getPositionX() + SetPos.x, NowStar->getPositionY() + SetPos.y));
	}
	for (auto NowHeart : AllHeart)
	{
		NowHeart->setPosition(Vec2(NowHeart->getPositionX() + SetPos.x, NowHeart->getPositionY() + SetPos.y));
	}
}


//射箭模块

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

	Person* model = static_cast<Person*>(this->getChildByTag(ModelTag));
	Vec2 NowPos = t->getLocation();
	Vec2 ModelPos = model->getPosition();
	float dir = Rocker::getRad(ModelPos, NowPos);
	dir = dir * 180 / 3.1415926;
	model->setRotation(-dir);

}
void SingleGameScene::ArrowEnded(Touch*t, Event*e)
{

	if (start == t->getLocation())return;
	Person* model = static_cast<Person*>(this->getChildByTag(ModelTag));
	std::string picture = model->weapon->picture;
	//创建一只箭，用人物的weapon对箭进行赋值
	auto arrow = Arrow::CreateArrow(picture);
	arrow->arrow_attack = model->weapon->arrow_attack;
	arrow->speed = model->weapon->speed;
	arrow->range = model->weapon->range*model->arrow_range;
	arrow->arrow_size = model->weapon->arrow_size;
	arrow->setPosition(model->getPosition());
	arrow->StartPosition = model->getPosition()+Vec2(height,height);
	arrow->master = model;
	float NowDir = -model->getRotation();
	while (NowDir < -180)
	{
		NowDir += 360;
	}
	arrow->dir = NowDir*3.1415926/180;
	AllArrow.pushBack(arrow);
	this->addChild(arrow);
}
void SingleGameScene::MoveArrow(float t)
{
	Vector<Arrow*>ToErase;
	for (auto nowArrow : AllArrow)
	{
		double dis = distance(nowArrow->getPosition(), nowArrow->StartPosition);
		if (dis >= nowArrow->range)
		{
			this->removeChild(nowArrow);
			ToErase.pushBack(nowArrow);
			continue;
		}
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
	}
}


//AI模块
void SingleGameScene::InitMonster()//在地图的四个角生成四个怪兽
{
	std::vector<Vec2>v;
	v.push_back(One);
	v.push_back(Two);
	v.push_back(Three);
	v.push_back(Four);
	for (auto x : v)
	{
		auto monster = Person::CreatePerson("monster.png");
		monster->setPosition(x);
		this->addChild(monster);
		AllPerson.pushBack(monster);
	}
}
void SingleGameScene::CreateMonster(float t)
{
	if (AllPerson.size() > MonsterNumber)
	{
		return;
	}
	GetPos();
	Person* model = static_cast<Person*>(getChildByTag(ModelTag));
	auto monster = Person::CreatePerson("monster.png");
	Vec2 pos;
	int maxv = 0;
	std::vector<Vec2>v;
	v.push_back(One);
	v.push_back(Two);
	v.push_back(Three);
	v.push_back(Four);
	for (auto x : v)
	{
		auto dis = distance(x, model->getPosition());
		if (dis > maxv)
		{
			maxv = dis;
			pos = x;
		}
	}
	monster->setPosition(pos);
	AllPerson.pushBack(monster);
	this->addChild(monster);
	
}
void SingleGameScene::MoveDirect(float t)
{
	
	auto model = getChildByTag(ModelTag);
	for (auto first:AllPerson)//怪物将选择朝与自己距离较近的
	{
	
		if (first == model)continue;
		bool flag = false;
		for (auto target : AllPerson)//尽量配对，形成两辆对抗的局面
		{
			if (target != first && hash_table.at(target) == first)
			{
				hash_table.insert(first, target);
				flag = true;
			}
		}
		double minv = 10000000000.0;
		if (!flag)
		{
			for (auto second : AllPerson)
			{
				if (first != second)
				{
					double dis = this->distance(first->getPosition(), second->getPosition());
					if (dis< minv)
					{
						hash_table.insert(first, second);//找到距离最小的点，并且该最小距离小于attack_area
						minv = dis;
					}
				}
			}
			if (minv < attack_area)
			{
				flag = true;
			}
			flag = true;
		}
		if (!flag)
		{
			hash_table.insert(first, NullPerson);
		}
	}
	
}
void SingleGameScene::Shoot(float t)
{
	auto model = getChildByTag(ModelTag);
	for (auto NowPerson : AllPerson)
	{
		if (NowPerson == model)continue;
		if (hash_table.at(NowPerson) != NullPerson)
		{
			auto target = hash_table.at(NowPerson);
			auto arrow = Arrow::CreateArrow("CloseNormal.png");
			arrow->setPosition(NowPerson->getPosition());
			arrow->StartPosition = NowPerson->getPosition();
			arrow->master = NowPerson;
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
	auto model = getChildByTag(ModelTag);
	int dx[4] = { -1,0,1,0 };
	int dy[4] = { 0,1,0,-1 };
	for (auto NowPerson:AllPerson)
	{
		if (NowPerson == model)continue;
		if (hash_table.at(NowPerson)!=NullPerson)
		{
			auto direct = hash_table.at(NowPerson);
			float dir = Rocker::getRad(NowPerson->getPosition(), direct->getPosition());
			float dx = NowPerson->speed * cos(dir);
			float dy = NowPerson->speed * sin(dir);
			Vec2 next_pos = Vec2(NowPerson->getPositionX() + height*cos(dir), NowPerson->getPositionY() + height*sin(dir));
			auto dis = distance(NowPerson->getPosition(), direct->getPosition());
			if (dis < min_attack_area)
			{
				next_pos= Vec2(NowPerson->getPositionX() - height*cos(dir), NowPerson->getPositionY() - height*sin(dir));
				dx = -dx, dy = -dy;
			}
			if (check(next_pos))
			{
				NowPerson->setPosition(Vec2(NowPerson->getPositionX()+dx,NowPerson->getPositionY()+dy));
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
		//}
		//else//如果value是NullPerson，随机移动
		//{
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

			/*
			int dir = random(0, 3);
			Vec2 next_pos = Vec2(NowPerson->getPositionX() + NowPerson->speed*dx[dir], NowPerson->getPositionY() + NowPerson->speed*dy[dir]);
			if (check(next_pos))
			{
				NowPerson->setPosition(next_pos);
			}
			*/
		}
	}
}
void SingleGameScene::Hurt(float t)
{
	Vector<Arrow*>ToErase;
	Person* model = static_cast<Person*>(getChildByTag(ModelTag));
	for (auto NowPerson : AllPerson)
	{
		Rect NowPerson_pos = Rect(NowPerson->getPositionX(), NowPerson->getPositionY(), height * 2, height * 2);
		for (auto NowArrow : AllArrow)
		{
			Rect NowArrow_pos = Rect(NowArrow->getPositionX(), NowArrow->getPositionY(), NowArrow->arrow_size, NowArrow->arrow_size);
			if (NowPerson_pos.intersectsRect(NowArrow_pos))//箭射中人
			{
			
				if (NowArrow->master == NowPerson)continue;
				NowArrow->master->person_score++;
				if (NowArrow->master == model)
				{
					score++;
					LabelScore->setString(String::createWithFormat("Score:%d", score)->_string);
				}
				NowPerson->blood -= NowArrow->arrow_attack*NowArrow->master->attack;//掉血
				float dx = NowArrow->speed*cos(NowArrow->dir);//受击后产生位移
				float dy = NowArrow->speed*sin(NowArrow->dir);
				Vec2 next_pos = Vec2(NowPerson->getPositionX() + dx, NowPerson->getPositionY() + dy);
				if (check(next_pos))
				{
					if (NowPerson == model)
					{
						map->setPosition(map->getPositionX() - dx, map->getPositionY() - dy);
						for (auto person : AllPerson)
						{
							if (person != model)
							{
								person->setPosition(person->getPositionX() - dx, person->getPositionY() - dy);
							}
						}
					}
					else NowPerson->setPosition(next_pos);
				}
				
				ToErase.pushBack(NowArrow);
				NowArrow->removeFromParent();
			}
		}
	}
	for (auto x : ToErase)//删除箭
	{
		AllArrow.eraseObject(x);
	}
}
void SingleGameScene::Dead(float t)
{
	Vector<Person*>ToErase;
	
	for (auto NowPerson : AllPerson)
	{
		if (NowPerson->blood <= 0)
		{
			auto Model = getChildByTag(ModelTag);
			if (NowPerson == Model)
			{
				log("Model is dead!!!");
			}
			auto blood = blood_hash.at(NowPerson);
			this->removeChild(blood);
			ToErase.pushBack(NowPerson);
			NowPerson->removeFromParent();
		}
	}
	for (auto x : ToErase)
	{
		AllPerson.eraseObject(x);
		auto res = hash_table.find(x);
		if (res != hash_table.cend())
		{
			hash_table.erase(hash_table.find(x));//把key是x的哈希删除
		}
		
	}
}

//物品模块

void SingleGameScene::MenuCallBack(cocos2d::Ref* pSender)
{

	auto NowItem = static_cast<MenuItem*>(pSender);//得到当前点击的菜单项
	int tag = NowItem->getTag();
	Person* model = static_cast<Person*>(getChildByTag(ModelTag));
	switch (tag)
	{
	case 1:
		if (!NowWeapon_1->is_weapon)//属性提升
		{
			if (NowWeapon_1->is_attack)
			{
				model->attack *= 1.2;
			}
			else if (NowWeapon_1->is_range)
			{
				model->weapon->range += 20;
			}
			else if (NowWeapon_1->is_speed)
			{
				model->speed += 1;
			}
			break;
		}
		ArrowCopy(model->weapon, NowWeapon_1);
		model->weapon->picture = "real_"+NowWeapon_1->picture;
		break;
	case 2:
		if (!NowWeapon_2->is_weapon)//属性提升
		{
			if (NowWeapon_2->is_attack)
			{
				model->attack *= 1.2;
			}
			else if (NowWeapon_2->is_range)
			{
				model->weapon->range += 20;
			}
			else if (NowWeapon_2->is_speed)
			{
				model->speed += 1;
			}
			break;
		}
		ArrowCopy(model->weapon, NowWeapon_2);
		model->weapon->picture = "real_" + NowWeapon_2->picture;
		break;
	case 3:
		if (!NowWeapon_3->is_weapon)//属性提升
		{
			if (NowWeapon_3->is_attack)
			{
				model->attack *= 1.2;
			}
			else if (NowWeapon_3->is_range)
			{
				model->weapon->range += 20;
			}
			else if (NowWeapon_3->is_speed)
			{
				model->speed += 1;
			}
			break;
		}
		ArrowCopy(model->weapon, NowWeapon_3);
		model->weapon->picture = "real_" + NowWeapon_3->picture;
		break;
	}
	Menu * M = static_cast<Menu*> (getChildByTag(MenuTag));
	M->removeFromParent();

}
void SingleGameScene::ShowBlood(float t)
{
	for (auto NowPerson : AllPerson)
	{
		Sprite* now_blood = static_cast<Sprite*>(blood_hash.at(NowPerson));
		if (now_blood)
		{
			this->removeChild(now_blood);
		}
		Sprite* new_blood = nullptr;
		int blood = NowPerson->blood;
		if (blood <= 100 && blood >= 80)
		{
			new_blood = Sprite::create("blood_5.png");
		}
		else if (blood < 80 && blood >= 60)
		{
			new_blood = Sprite::create("blood_4.png");
		}
		else if (blood < 60 && blood >= 40)
		{
			new_blood = Sprite::create("blood_3.png");
		}
		else if (blood < 40 && blood >= 20)
		{
			new_blood = Sprite::create("blood_2.png");
		}
		else
		{
			new_blood = Sprite::create("blood_1.png");
		}
		new_blood->setPosition(Vec2(NowPerson->getPositionX(), NowPerson->getPositionY() + DY));
		blood_hash.insert(NowPerson, new_blood);
		this->addChild(new_blood);
	}
}
void SingleGameScene::ChangeWeapon(float t)
{
	srand(time(NULL));
	Person* model = static_cast<Person*>(getChildByTag(ModelTag));
	if (!model)
	{
		log("wtf");
		return;
	}
	if (model->blue < 100)
	{
		return;
	}
	Vector<MenuItem*>v;
	for (int i = 1; i <= 3; i++)
	{

		int num = random(0, AllWeapon.size());
		Vector<Arrow*>::iterator it = AllWeapon.begin();
		while (num--)
		{
			it++;
		}
		auto menu = MenuItemImage::create((*it)->picture, (*it)->picture, CC_CALLBACK_1(SingleGameScene::MenuCallBack, this));
		if (i == 1)
		{
			menu->setPosition(Vec2(model->getPositionX() - 600, model->getPositionY() - 350));
			menu->setTag(1);
			ArrowCopy(NowWeapon_1, *it);
		}
		else if (i == 2)
		{
			menu->setPosition(Vec2(model->getPositionX() - 500, model->getPositionY() - 350));
			menu->setTag(2);
			ArrowCopy(NowWeapon_2, *it);
		}
		else
		{
			menu->setPosition(Vec2(model->getPositionX() - 400, model->getPositionY() - 350));
			menu->setTag(3);
			ArrowCopy(NowWeapon_3, *it);
		}
		v.pushBack(menu);
	}

	auto M = Menu::createWithArray(v);
	M->setTag(MenuTag);
	this->addChild(M);
	model->blue = 0;

}
void SingleGameScene::InitHeartStar()
{
	//创建25个星星和25个心，每一队五个
	for (int i = 1; i <= 40; i++)
	{
		CreateHeartStar("heart.png");
		CreateHeartStar("star.png");
	}
	
}
void SingleGameScene::CreateHeartStar(const std::string &filename)
{
	srand(counts++);
	int dx[2] = { 30,0 };
	int dy[2] = { 0,30};
	int star_dir = random(0,2);
	
	GetPos();
	Vec2 anchor = map->getPosition();
	Vec2 star_start = Vec2(random(Two.x,One.x), random(Three.y,One.y));
	for (int k = 0; k < 5; k++)
	{
		Sprite* sprite = Sprite::create(filename);
		float x = star_start.x + dx[star_dir] * k;
		float y = star_start.y + dy[star_dir] * k;
		sprite->setPosition(Vec2(x, y));
		if (check(Vec2(x, y))&& IsInBound(Vec2(x, y)))
		{

			this->addChild(sprite);
			if (filename == "star.png")
			{
				AllStar.pushBack(sprite);
			}
			else AllHeart.pushBack(sprite);
		}
	}
}
void SingleGameScene::SupplyHeartStar(float t)
{

	if (AllHeart.size() > 170 && AllStar.size() > 170)return;
	if (AllHeart.size() < 170)
	{
		CreateHeartStar("heart.png");
	}
	if (AllStar.size() < 170)
	{
		CreateHeartStar("star.png");
	}
}


//初始化模块
Scene* SingleGameScene::CreateScene()
{
	auto scene = Scene::create();
	auto layer = SingleGameScene::create();
	scene->addChild(layer);
	return scene;
}
/*
void SingleGameScene::InitAllPoint(TMXTiledMap*map)
{
	Vec2 map_pos = map->getPosition();
	int height = map->getTileSize().height*map->getMapSize().height;
	int width = map->getTileSize().width*map->getMapSize().width;
	//test
	ValueVector value;
	value = map->getObjectGroup("obj")->getObjects();//得到对象层数据
	for (int i = 0; i < value.size(); i++)
	{
		ValueMap val = value.at(i).asValueMap();//获取第i个点
		TDpoint *newp = TDpoint::createPoint(val.at("x").asInt(), val.at("y").asInt());
		allpoint.pushBack(newp);
	}

	
}
*/
void SingleGameScene::InitValue()
{
	this->rockerBG_Position = Vec2(150, 130);
	this->current_point = rockerBG_Position;
	this->MonsterNumber = 5;
	this->score = 0;
	height = 20;
	attack_area = 500;
	min_attack_area = 250;
	counts = 0;
	LabelScore= Label::createWithTTF("Score:0", "fonts/Marker Felt.ttf", 32);
	LabelScore->setPosition(Vec2(100, 400));
	LabelScore->setColor(Color3B::RED);
	LabelMagic = Label::createWithTTF("Magic:0", "fonts/Marker Felt.ttf", 32);
	LabelMagic->setPosition(Vec2(100, 350));
	LabelMagic->setColor(Color3B::RED);
	this->addChild(LabelScore);
	this->addChild(LabelMagic);
	NullPerson = Person::CreatePerson("person.png");
	NullPerson->retain();
	NowWeapon_1 = Arrow::CreateArrow("CloseNormal.png");
	NowWeapon_1->retain();
	NowWeapon_2 = Arrow::CreateArrow("CloseNormal.png");
	NowWeapon_2->retain();
	NowWeapon_3 = Arrow::CreateArrow("CloseNormal.png");
	NowWeapon_3->retain();
}
void SingleGameScene::initWeapon()
{
	//斧子
	auto ax = Arrow::CreateArrow("ax.png");
	ax->arrow_attack = 20;
	ax->speed = 8;
	ax->range = 200;
	ax->retain();
	AllWeapon.pushBack(ax);

	//火焰
	auto fire = Arrow::CreateArrow("fire.png");
	fire->arrow_attack = 15;
	fire->speed = 20;
	fire->range = 200;
	fire->retain();
	AllWeapon.pushBack(fire);

	//激光
	auto laser = Arrow::CreateArrow("laser.png");
	laser->arrow_attack = 30;
	laser->speed = 5;
	laser->range = 150;
	laser->retain();
	AllWeapon.pushBack(laser);

	//飞镖
	auto kuwu = Arrow::CreateArrow("kuwu.png");
	kuwu->arrow_attack = 12;
	kuwu->speed = 25;
	kuwu->range = 350;
	kuwu->retain();
	AllWeapon.pushBack(kuwu);

	//电磁球
	auto ball = Arrow::CreateArrow("ball.png");
	ball->arrow_attack = 20;
	ball->speed = 20;
	ball->range = 140;
	ball->retain();
	AllWeapon.pushBack(ball);

	//攻击力提升
	auto attack = Arrow::CreateArrow("attack.png");
	attack->is_weapon = false;
	attack->is_attack = true;
	attack->retain();
	AllWeapon.pushBack(attack);

	//防御力提升
	auto defence = Arrow::CreateArrow("defence.png");
	defence->is_weapon = false;
	defence->retain();
	AllWeapon.pushBack(defence);

	//射程提升
	auto range = Arrow::CreateArrow("range.png");
	range->is_weapon = false;
	range->is_range = true;
	range->retain();
	AllWeapon.pushBack(range);

	//速度提升
	auto speed = Arrow::CreateArrow("speed.png");
	speed->is_weapon = false;
	speed->is_speed = true;
	speed->retain();
	AllWeapon.pushBack(speed);
}
void SingleGameScene::InitMap()
{
	//墙壁
	for (int i = 3; i <= 90; i++)
	{
		StopCheck.insert(Vec2(3, i));
	}
	for (int i = 3; i <= 96; i++)
	{
		StopCheck.insert(Vec2(i, 3));
	}
	for (int i = 3; i <= 96; i++)
	{
		StopCheck.insert(Vec2(96, i));
	}
	for (int i = 3; i <= 96; i++)
	{
		StopCheck.insert(Vec2(i, 90));
	}

	//障碍物
	
	addrow(8, 11, 23, 32, 44, 55, 68, 78, 91);
	addrow(26, 11, 23, 32, 44, 56, 68, 78, 91);
	addrow(44, 15, 27, 36, 51, 62, 75, 81, 93);
	addrow(84, 20, 45, 58, 84,  99, 99, 99, 99);
	addcol(3,13,17,38,62,84);
	addcol(20,32,17,38,62,84);
	addcol(36,51,24,44,73,83);
	addcol(62, 84,20,45,58,84);
	for (int i = 53; i <= 88; i++) StopCheck.insert(Vec2(i, 52));
}
void SingleGameScene::addrow(int row, int col1, int col2, int col3, int col4, int col5, int col6, int col7, int col8)
{
	for (int i = col1; i <= col2; i++)
	{
		StopCheck.insert(Vec2(row, i));
	}
	for (int i = col3; i <= col4; i++)
	{
		StopCheck.insert(Vec2(row, i));
	}
	for (int i = col5; i <= col6; i++)
	{
		StopCheck.insert(Vec2(row, i));
	}
	for (int i = col7; i <= col8; i++)
	{
		StopCheck.insert(Vec2(row, i));
	}
}
void SingleGameScene::addcol(int row1, int row2, int col1, int col2, int col3, int col4)
{
	for (int i = row1; i <= row2; i++)
	{
		StopCheck.insert(Vec2(i, col1));
		StopCheck.insert(Vec2(i, col2));
		StopCheck.insert(Vec2(i, col3));
		StopCheck.insert(Vec2(i, col4));
	}
}