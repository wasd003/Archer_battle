#include "Arrow.h"
Arrow* Arrow::CreateArrow(const std::string &filename)
{
	Arrow *sprite = Arrow::create();
	if (sprite)
	{
		sprite->InitArrow(filename);
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
void Arrow:: InitArrow(const std::string &filename)
{
	auto sprite = Sprite::create(filename);
	this->addChild(sprite);
	this->arrow_attack = 10;
	this->arrow_size = 40;
	this->range = 250;
	this->picture = filename;
	this->is_weapon = true;
	this->is_attack = false;
	this->is_range = false;
	this->is_speed = false;
	dir = 0.0;
	this->master = nullptr;
	speed = 10;
}
inline double Arrow::distance(Vec2 pos1, Vec2 pos2)
{
	double ans = (pos1.x - pos2.x)*(pos1.x - pos2.x) + (pos1.y - pos2.y)*(pos1.y - pos2.y);
	return sqrt(ans);
}
