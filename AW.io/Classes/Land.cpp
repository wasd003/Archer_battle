#include "Land.h"
#include "HelloWorldScene.h"
#include "SingleGameScene.h"
#include "MultiGameScene.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

Scene* Land::CreateScene()
{
	auto scene = Scene::create();
	auto layer = Land::create();
	scene->addChild(layer);
	return scene;
}
bool Land::init()
{
	if (!Layer::init())return false;


	auto EndBG = Sprite::create("background.png");
	EndBG->setPosition(Vec2(480, 240));
	this->addChild(EndBG);
	
	//输入姓名编辑框
	auto NameField = cocos2d::ui::TextField::create("input you name", "Arial", 30);
	NameField->setMaxLengthEnabled(true);
	NameField->setMaxLength(8);
	NameField->setPosition(Vec2(480, 400));
	NameField->addEventListener(CC_CALLBACK_2(Land::NameFieldEvent, this));
	this->addChild(NameField);

	//输入服务器IP地址
	auto IpField = cocos2d::ui::TextField::create("input Server IP", "Arial", 30);
	IpField->setMaxLengthEnabled(true);
	IpField->setMaxLength(24);
	IpField->setPosition(Vec2(480, 200));
	IpField->addEventListener(CC_CALLBACK_2(Land::IpFieldEvent, this));
	this->addChild(IpField);
	

	
	return true;
}

void Land::NameFieldEvent(Ref *pSender, cocos2d::ui::TextField::EventType type)
{
	switch (type)
	{
	case cocos2d::ui::TextField::EventType::ATTACH_WITH_IME://点击编辑框
	{
	}
	break;

	case cocos2d::ui::TextField::EventType::DETACH_WITH_IME://点击编辑框的其他位置
	{
		auto  s = static_cast<cocos2d::ui::TextField*>(pSender);
		this->name = s->getString();
	}
	break;

	case cocos2d::ui::TextField::EventType::INSERT_TEXT://键盘输入
	{
	}
	break;

	case cocos2d::ui::TextField::EventType::DELETE_BACKWARD://键盘删除
	{
	}
	break;

	default:
		break;
	}
}
void Land::IpFieldEvent(Ref *pSender, cocos2d::ui::TextField::EventType type)
{
	switch (type)
	{
	case cocos2d::ui::TextField::EventType::ATTACH_WITH_IME://点击编辑框
	{
	}
	break;

	case cocos2d::ui::TextField::EventType::DETACH_WITH_IME://点击编辑框的其他位置
	{
		auto  s = static_cast<cocos2d::ui::TextField*>(pSender);
		this->IP = s->getString();
		auto scene = Scene::create();
		auto layer = MultiGameScene::create();
		layer->Ip = this->IP;
		layer->RoleModel->name = this->name;
		layer->connect();
		scene->addChild(layer);
		Director::getInstance()->replaceScene(scene);
	}
	break;

	case cocos2d::ui::TextField::EventType::INSERT_TEXT://键盘输入
	{
	}
	break;

	case cocos2d::ui::TextField::EventType::DELETE_BACKWARD://键盘删除
	{
	}
	break;

	default:
		break;
	}
}
