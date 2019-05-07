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
	auto winsize = Director::getInstance()->getWinSize();
	auto sprite = Sprite::create(filename);
	this->addChild(sprite);
	this->arrow_attack = 20;
	dir = 0.0;
	speed = 10;
}