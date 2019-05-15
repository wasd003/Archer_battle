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
	this->arrow_attack = 1;
	this->arrow_size = 40;
	this->range = 250;
	dir = 0.0;
	this->master = nullptr;
	speed = 10;
}