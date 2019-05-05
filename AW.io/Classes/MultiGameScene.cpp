#include "MultiGameScene.h"
#include "HelloWorldScene.h"
bool MultiGameScene::init()
{
	return true;
}
Scene* MultiGameScene::CreateScene()
{
	auto scene = Scene::create();
	auto layer = MultiGameScene::create();
	scene->addChild(layer);
	return scene;
}