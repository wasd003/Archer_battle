#include "Person.h"
Person* Person::CreatePerson(const std::string& filename)
{
	Person* sprite = Person::create();
	if (sprite)
	{
		sprite->InitPerson(filename);
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;

}
void Person::InitPerson(const std::string &filename)
{
	auto sprite = Sprite::create(filename);
	this->addChild(sprite);
	blood = 100;
	max_blood = 100;
	blue = 0;
	attack = 1.0;
	defence = 1.0;
	speed = 3;
	weapon = Arrow::CreateArrow("CloseNormal.png");
	weapon->retain();
	if (this->weapon)
	{
		this->weapon->arrow_attack = 10;
	}
}