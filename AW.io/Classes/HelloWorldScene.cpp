#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "SingleGameScene.h"
#include "Land.h"
#include "MultiGameScene.h"
#include "HelpScene.h"
#include "StoreScene.h"
USING_NS_CC;
//创建场景
Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// 打印异常
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

   //添加关闭菜单
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }
	//添加四种功能菜单
	auto SingleGameItem = MenuItemFont::create("Start SingleGame", CC_CALLBACK_1(HelloWorld::menuCallback, this));//单人游戏
	auto MultiGameItem = MenuItemFont::create("Start MultiGame", CC_CALLBACK_1(HelloWorld::menuCallback, this));//多人游戏
	auto HelpItem = MenuItemFont::create("Help", CC_CALLBACK_1(HelloWorld::menuCallback, this));//帮助
	auto StoreItem = MenuItemFont::create("Store", CC_CALLBACK_1(HelloWorld::menuCallback, this));//商店
	SingleGameItem->setPosition(Point(origin.x + visibleSize.width / 2 - closeItem->getContentSize().width / 2, 200));//为菜单设置位置
	MultiGameItem->setPosition(Point(origin.x + visibleSize.width / 2 - closeItem->getContentSize().width / 2, 150));
	HelpItem->setPosition(Point(origin.x + visibleSize.width / 2 - closeItem->getContentSize().width / 2, 100));
	StoreItem->setPosition(Point(origin.x + visibleSize.width / 2 - closeItem->getContentSize().width / 2, 50));
	SingleGameItem->setTag(SingleGameTag);//为菜单设置标签
	MultiGameItem->setTag(MultiGameTag);
	HelpItem->setTag(HelpTag);
	StoreItem->setTag(StoreTag);
	// 创建menu
	auto menu = Menu::create(closeItem, SingleGameItem, MultiGameItem, HelpItem, StoreItem,NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	//创建label 欢迎来到 《弓箭手大作战》
    auto label = Label::createWithTTF("Welcome To Arrow War", "fonts/Marker Felt.ttf", 24);
	label->setColor(Color3B::RED);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

   //创建初始场景的背景图片
    auto sprite = Sprite::create("background.png");
    if (sprite == nullptr)
    {
        problemLoading("'HelloWorld.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}

void HelloWorld::menuCallback(Ref* pSender)
{
	auto NowItem = static_cast<MenuItem*>(pSender);
	int tag = NowItem->getTag();
	switch (tag)
	{
	case SingleGameTag:
		Director::getInstance()->replaceScene(SingleGameScene::CreateScene());
		break;
	case MultiGameTag:
		Director::getInstance()->replaceScene(Land::CreateScene());
		break;
	case HelpTag:
		Director::getInstance()->replaceScene(HelpScene::CreateScene());
		break;
	case StoreTag:
		Director::getInstance()->replaceScene(StoreScene::CreateScene());
		break;
	}
}
