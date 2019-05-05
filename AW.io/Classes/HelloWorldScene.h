#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void menuCallback(cocos2d::Ref*pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	static const int SingleGameTag=10;//四种跳转场景的标签
	static const int MultiGameTag=11;
	static const int HelpTag=12;
	static const int StoreTag=13;
};

#endif // __HELLOWORLD_SCENE_H__
