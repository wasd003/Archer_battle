#include "Rocker.h"
USING_NS_CC;
Rocker* Rocker::create(const std::string& RockerImage, const std::string& RockerBackImage,Vec2 posBG,Vec2 pos)
{
	static Rocker*layer = new Rocker();
	if (layer)
	{
		layer->set();
		Sprite* rocker = Sprite::create(RockerImage);
		auto rockerBG = Sprite::create(RockerBackImage);
		rocker->setPosition(pos);
		rockerBG->setPosition(posBG);
		rocker->setTag(tag_rocker);
		rockerBG->setTag(tag_rockerBG);
		layer->addChild(rockerBG);
		layer->addChild(rocker);
		
		return layer;
	}
	else
	{
		CC_SAFE_DELETE(layer);
		return nullptr;
	}
}
void Rocker::set()
{
	this->R =139.0;
	return;
}

void Rocker::StartRocker()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Rocker::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Rocker::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Rocker::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
bool Rocker::onTouchBegan(Touch* t, Event* e)
{
	auto rockerBG = static_cast<Sprite*>(getChildByTag(tag_rockerBG));
	Vec2 point = t->getLocation();//触摸点坐标
	if (rockerBG->getBoundingBox().containsPoint(point))
	{
		return true;
	}
	else return false;

}

void Rocker::onTouchMoved(Touch* t, Event* e)
{
	Vec2 point = t->getLocation();//触摸点坐标
	auto rockerBG = static_cast<Sprite*>(getChildByTag(tag_rockerBG));
	auto rocker = static_cast<Sprite*>(getChildByTag(tag_rocker));
	if (rockerBG->getBoundingBox().containsPoint(point))
	{
		rocker->setPosition(point);
	}
	else
	{
		float dir = this->getRad(rockerBG->getPosition(), point);
		rocker->setPosition(Vec2(rockerBG->getPositionX() + this->R*cos(dir), rockerBG->getPositionY() + this->R*sin(dir)));
	}
}
void Rocker::onTouchEnded(Touch* t, Event *e)
{
	auto rocker = static_cast<Sprite*>(getChildByTag(tag_rocker));
	auto rockerBG = static_cast<Sprite*>(getChildByTag(tag_rockerBG));
	Vec2 pos = rockerBG->getPosition();
	rocker->setPosition(Vec2(130,130));
	return;
}
float Rocker::getRad(Vec2 pos1, Vec2 pos2)
{
	if (pos1 == pos2)return 0;
	float dx = pos2.x - pos1.x;
	float dy = pos2.y - pos1.y;
	float dis = sqrt(dx*dx+dy*dy);
	float res = acos(dx / dis);
	if (dy < 0)res = -res;
	return res;
}