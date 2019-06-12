#include "GameOverScene.h"
#include "HelloWorldScene.h"
#include "SingleGameScene.h"
#include "MultiGameScene.h"
#include "Land.h"
USING_NS_CC;

Scene* GameOverScene::CreateScene()
{
	auto scene = Scene::create();
	auto layer = GameOverScene::create();
	scene->addChild(layer);
	return scene;
}
bool GameOverScene::init()
{
	if (!Layer::init())return false;
	
	auto EndBG = Sprite::create("background.png");
	EndBG->setPosition(Vec2(480, 240));
	this->addChild(EndBG);
	auto SingleGameItem = MenuItemFont::create("start SingleGame again", [&](Ref*) {
		Director::getInstance()->replaceScene(SingleGameScene::CreateScene());
	});//单人游戏
	auto MultiGameItem = MenuItemFont::create("start MultiGame again", [&](Ref*) {
		Director::getInstance()->replaceScene(Land::CreateScene());
	});//多人游戏
	auto closeItem = MenuItemFont::create("Close",
		[&](Ref* sender) {
		Director::getInstance()->end();
	});
	SingleGameItem->setPosition(Vec2(480, 400));
	MultiGameItem->setPosition(Vec2(480, 300));
	closeItem->setPosition(Vec2(480, 200));
	auto menu = Menu::create(closeItem, SingleGameItem, MultiGameItem,NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}
