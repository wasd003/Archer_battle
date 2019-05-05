#include "Arrow.h"
void Arrow::set()
{
	arrow_attack=20;
	dir =0.0;
	speed=5;
}
Arrow* Arrow::create(const std::string &filename)
{
	static Arrow* sprite = new Arrow();
	if (sprite&&sprite->initWithFile(filename))
	{
		sprite->autorelease();
		sprite->set();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}