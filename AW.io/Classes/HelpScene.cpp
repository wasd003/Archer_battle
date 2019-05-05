#include "HelpScene.h"
#include "HelloWorldScene.h"
bool HelpScene::init()
{
	return true;
}
Scene* HelpScene::CreateScene()
{
	auto scene = Scene::create();
	auto layer = HelpScene::create();
	scene->addChild(layer);
	return scene;
}