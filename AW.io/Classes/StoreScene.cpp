#include "StoreScene.h"
#include "HelloWorldScene.h"
bool StoreScene::init()
{
	return true;
}
 Scene* StoreScene::CreateScene()
{
	auto scene = Scene::create();
	auto layer = StoreScene::create();
	scene->addChild(layer);
	return scene;
}