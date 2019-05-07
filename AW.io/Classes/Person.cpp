#include "Person.h"
Person* Person::create(const std::string& filename)
{
	static Person* sprite = new Person;
	if (sprite&&sprite->initWithFile(filename))
	{
		sprite->autorelease();
		sprite->set();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;

}

void Person::set()
{
	blood = 100;
	max_blood = 100;
	blue = 0;
	attack = 1.0;
	defence = 1.0;
	speed = 3;
	this->weapon.arrow_attack = 10;
}
