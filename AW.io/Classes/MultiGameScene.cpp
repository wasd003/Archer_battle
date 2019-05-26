#include "MultiGameScene.h"
#include "HelloWorldScene.h"
#include"ODsocket.h"
#include<pthread.h>
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#pragma comment(lib, "pthreadVC2.lib")
using namespace  rapidjson;
using namespace std;
USING_NS_CC;
static MultiGameScene* now;

Scene* MultiGameScene::CreateScene()
{
	auto scene = Scene::create();
	auto layer = MultiGameScene::create();
	scene->addChild(layer);
	return scene;
}
bool MultiGameScene::init()
{
	if (!Layer::init())return false;
	//初始化
	auto winsize = Director::getInstance()->getWinSize();
	this->InitValue();
	this->initWeapon();
	this->map = TMXTiledMap::create("Tank2.tmx");
	map->setPosition(Vec2(224, -2074));
	this->addChild(map, -1);
	GetPos();
	InitMap();
	/*
	auto label = LabelTTF::create("HelloWorld", "Arial", 24);
	label->setTag(111);
	label->setPosition(Vec2(480, 240));
	addChild(label, 1);
	*/
#if 0
	//联网
	now = this;
	sock_client = new ODsocket();
	sock_client->Init();
	bool res = sock_client->Create(AF_INET, SOCK_STREAM, 0);
	res = sock_client->Connect("192.168.1.103", 8867);
	if (res)
	{
		pthread_t gost;
		bool ok = pthread_create(&gost, NULL, MultiGameScene::gostMessage, NULL);//创建发送信息的线程
		pthread_t get;
		bool ok = pthread_create(&get, NULL, MultiGameScene::getMessage, NULL);//创建接受信息的线程
	}
#endif
	//创建主角
	auto model = Person::CreatePerson("person.png");
	model->setTag(ModelTag);
	this->addChild(model);
	model->setPosition(Vec2(winsize.width / 2, winsize.height / 2));
	AllPerson.pushBack(model);
	//加载所有玩家
	InitAllPerson();
	//创建星星和心
	InitHeartStar();
	//设置摇杆
	auto rocker = Rocker::create("CloseSelected.png", "rr.png", rockerBG_Position);
	rocker->setTag(RockerTag);
	this->addChild(rocker);
	rocker->StartRocker();

	
//待定
	this->schedule(schedule_selector(SingleGameScene::MoveArrow, this));
	this->schedule(schedule_selector(SingleGameScene::Hurt, this));
	this->schedule(schedule_selector(SingleGameScene::Dead, this));

	//星星和心的补充
	this->schedule(schedule_selector(SingleGameScene::SupplyHeartStar, this));

	//控制人物移动
	auto MoveListen = EventListenerTouchOneByOne::create();
	MoveListen->setSwallowTouches(true);
	MoveListen->onTouchBegan = CC_CALLBACK_2(SingleGameScene::MoveBegan, this);
	MoveListen->onTouchMoved = CC_CALLBACK_2(SingleGameScene::MoveMoved, this);
	MoveListen->onTouchEnded = CC_CALLBACK_2(SingleGameScene::MoveEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(MoveListen, this);
	this->schedule(schedule_selector(SingleGameScene::MovePerson, this));


	//控制射箭
	auto ArrowListen = EventListenerTouchOneByOne::create();
	ArrowListen->onTouchBegan = CC_CALLBACK_2(SingleGameScene::ArrowBegan, this);
	ArrowListen->onTouchMoved = CC_CALLBACK_2(SingleGameScene::ArrowMoved, this);
	ArrowListen->onTouchEnded = CC_CALLBACK_2(SingleGameScene::ArrowEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(ArrowListen, this);
	this->schedule(schedule_selector(SingleGameScene::MoveArrow, this));


	//显示血量
	this->schedule(schedule_selector(SingleGameScene::ShowBlood, this));
	this->schedule(schedule_selector(SingleGameScene::ChangeWeapon, this));


	return true;
}





/*
//初始化系列函数
void MultiGameScene::InitValue()
{
	//创建主角
	model = Person::CreatePerson("person.png");
	this->addChild(model);
	model->setPosition(Vec2(winsize.width / 2, winsize.height / 2));
	AllPerson.pushBack(model);
	this->rockerBG_Position = Vec2(100, 100);
	this->current_point = rockerBG_Position;
	this->score = 0;
	height = 20;
	counts = 0;
	LabelScore = Label::createWithTTF("Score:0", "fonts/Marker Felt.ttf", 32);
	LabelScore->setPosition(Vec2(100, 400));
	LabelScore->setColor(Color3B::RED);
	LabelMagic = Label::createWithTTF("Magic:0", "fonts/Marker Felt.ttf", 32);
	LabelMagic->setPosition(Vec2(100, 350));
	LabelMagic->setColor(Color3B::RED);
	this->addChild(LabelScore);
	this->addChild(LabelMagic);
	NowWeapon_1 = Arrow::CreateArrow("CloseNormal.png");
	NowWeapon_1->retain();
	NowWeapon_2 = Arrow::CreateArrow("CloseNormal.png");
	NowWeapon_2->retain();
	NowWeapon_3 = Arrow::CreateArrow("CloseNormal.png");
	NowWeapon_3->retain();
}
void MultiGameScene::initWeapon()
{
	//斧子
	auto ax = Arrow::CreateArrow("ax.png");
	ax->arrow_attack = 20;
	ax->speed = 8;
	ax->range = 200;
	ax->picture = "ax.png";
	ax->retain();
	AllWeapon.pushBack(ax);

	//火焰
	auto fire = Arrow::CreateArrow("fire.png");
	fire->arrow_attack = 15;
	fire->speed = 20;
	fire->range = 200;
	fire->picture = "fire.png";
	fire->retain();
	AllWeapon.pushBack(fire);

	//激光
	auto laser = Arrow::CreateArrow("laser.png");
	laser->arrow_attack = 30;
	laser->speed = 5;
	laser->range = 50;
	laser->picture = "laser.png";
	laser->retain();
	AllWeapon.pushBack(laser);
}
void MultiGameScene::InitMap()
{
	for (int i = 3; i <= 90; i++)
	{
		StopCheck.insert(Vec2(3, i));
	}
	for (int i = 3; i <= 96; i++)
	{
		StopCheck.insert(Vec2(i, 3));
	}
	for (int i = 3; i <= 96; i++)
	{
		StopCheck.insert(Vec2(96, i));
	}
	for (int i = 3; i <= 96; i++)
	{
		StopCheck.insert(Vec2(i, 90));
	}
	for (int i = 20; i <= 70; i++)
	{
		StopCheck.insert(Vec2(85, i));
	}
	for (int i = 80; i <= 90; i++)
	{
		StopCheck.insert(Vec2(i, 32));
		StopCheck.insert(Vec2(i, 63));
	}
}
*/


//联网模块
void* MultiGameScene::postMessage(void*)
{
	//MessageToPost = "Test";
	//sock_client->Send((char*)MessageToPost.c_str(), MessageToPost.length(), 0);
	return NULL;
}
void* MultiGameScene::getMessage(void *)

{
	char buffer[1024];
	while (1)
	{	
		memset(buffer, 0, sizeof(buffer));
		now->sock_client->Recv(buffer, sizeof(buffer));
		now->strmsg = buffer;
	}
}
void MultiGameScene::StringToData(const std::string &s)
{
	//std::string strValue = "{\"key1\":\"value1\",\"array\":[{\"key2\":\"value2\"},{\"key2\":\"value3\"},{\"key2\":\"value4\"}]}";
	std::string str_json = "{\"success\": true, \"code\": 0, \"result\": {\"order\": {\"title\": \"标题\", \"detail\": \"订单详情\", \"extends\": \"扩展内容\", \"time_start\": \"1518338445\", \"time_expire\": \"1518438445\"}, \"order_id\": \"20180211-108170\"}}";
	CCLOG("%s\n", str_json.c_str());
	rapidjson::Document document;
	document.Parse<0>(str_json.c_str());//将字符串加载进document中
	if (document.HasParseError()) { //打印解析错误
		CCLOG("GetParseError %s\n", document.GetParseError());
	}
	if (document.IsObject()) {
		if (document.HasMember("success")) {
			CCLOG("%s\n", document["success"].GetBool()); //打印获取 success 的值
		}

		if (document.HasMember("result") && document["result"].HasMember("order_id")) {
			CCLOG("%s\n", document["result"]["order_id"].GetString()); //打印获取 order_id 的值
		}
	}
}
void MultiGameScene::DataToString()
{
	rapidjson::Document document;//创建文档
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	rapidjson::Value array(rapidjson::kArrayType);
	rapidjson::Value object(rapidjson::kObjectType);
	object.AddMember("int", 1, allocator);
	object.AddMember("double", 1.0, allocator); 
	object.AddMember("bool", true, allocator); 
	object.AddMember("hello", "你好", allocator); 
	array.PushBack(object, allocator);

	document.AddMember("json", "json string", allocator);
	document.AddMember("array", array, allocator);

	StringBuffer buffer;
	rapidjson::Writer<StringBuffer> writer(buffer);
	document.Accept(writer);

	CCLOG("%s", buffer.GetString());
}
void MultiGameScene::update(float t)
{
	LabelTTF* label = (LabelTTF*)getChildByTag(111);
	label->setString(now->strmsg.c_str());
}

/*
//物品模块
void MultiGameScene::ShowBlood(float t)
{
	for (auto NowPerson : AllPerson)
	{
		Sprite* now_blood = static_cast<Sprite*>(blood_hash.at(NowPerson));
		if (now_blood)
		{
			this->removeChild(now_blood);
		}
		Sprite* new_blood = nullptr;
		int blood = NowPerson->blood;
		if (blood <= 100 && blood >= 80)
		{
			new_blood = Sprite::create("blood_5.png");
		}
		else if (blood < 80 && blood >= 60)
		{
			new_blood = Sprite::create("blood_4.png");
		}
		else if (blood < 60 && blood >= 40)
		{
			new_blood = Sprite::create("blood_3.png");
		}
		else if (blood < 40 && blood >= 20)
		{
			new_blood = Sprite::create("blood_2.png");
		}
		else
		{
			new_blood = Sprite::create("blood_1.png");
		}
		new_blood->setPosition(Vec2(NowPerson->getPositionX(), NowPerson->getPositionY() + DY));
		blood_hash.insert(NowPerson, new_blood);
		this->addChild(new_blood);
	}
}
void MultiGameScene::ChangeWeapon(float t)
{
	srand(time(NULL));
	Person* model = static_cast<Person*>(getChildByTag(ModelTag));
	if (!model)
	{
		log("wtf");
	}
	if (model->blue < 100)
	{
		//model->blue += 5;
		//log("blue:%d", model->blue);
		return;
	}
	Vector<MenuItem*>v;
	for (int i = 1; i <= 3; i++)
	{

		int num = random(0, AllWeapon.size());
		Vector<Arrow*>::iterator it = AllWeapon.begin();
		while (num--)
		{
			it++;
		}
		auto menu = MenuItemImage::create((*it)->picture, (*it)->picture, CC_CALLBACK_1(SingleGameScene::MenuCallBack, this));
		if (i == 1)
		{
			menu->setPosition(Vec2(model->getPositionX() - 600, model->getPositionY() - 350));
			menu->setTag(1);
			ArrowCopy(NowWeapon_1, *it);
		}
		else if (i == 2)
		{
			menu->setPosition(Vec2(model->getPositionX() - 500, model->getPositionY() - 350));
			menu->setTag(2);
			ArrowCopy(NowWeapon_2, *it);
		}
		else
		{
			menu->setPosition(Vec2(model->getPositionX() - 400, model->getPositionY() - 350));
			menu->setTag(3);
			ArrowCopy(NowWeapon_3, *it);
		}
		v.pushBack(menu);
	}

	auto M = Menu::createWithArray(v);
	M->setTag(MenuTag);
	this->addChild(M);
	model->blue = 0;
}
void MultiGameScene::MenuCallBack(cocos2d::Ref*pSender)
{
	auto NowItem = static_cast<MenuItem*>(pSender);//得到当前点击的菜单项
	int tag = NowItem->getTag();
	Person* model = static_cast<Person*>(getChildByTag(ModelTag));
	switch (tag)
	{
	case 1:
		ArrowCopy(model->weapon, NowWeapon_1);
		break;
	case 2:
		ArrowCopy(model->weapon, NowWeapon_2);
		break;
	case 3:
		ArrowCopy(model->weapon, NowWeapon_3);
		break;
	}
	Menu * M = static_cast<Menu*> (getChildByTag(MenuTag));
	M->removeFromParent();
}
void MultiGameScene::InitHeartStar()
{
	for (int i = 1; i <= 40; i++)
	{
		CreateHeartStar("heart.png");
		CreateHeartStar("star.png");
	}
}
void MultiGameScene::CreateHeartStar(const std::string &filename)
{
	srand(counts++);
	int dx[2] = { 30,0 };
	int dy[2] = { 0,30 };
	int star_dir = random(0, 2);

	GetPos();
	Vec2 anchor = map->getPosition();
	Vec2 star_start = Vec2(random(Two.x, One.x), random(Three.y, One.y));
	for (int k = 0; k < 5; k++)
	{
		Sprite* sprite = Sprite::create(filename);
		float x = star_start.x + dx[star_dir] * k;
		float y = star_start.y + dy[star_dir] * k;
		sprite->setPosition(Vec2(x, y));
		if (check(Vec2(x, y)) && IsInBound(Vec2(x, y)))
		{

			this->addChild(sprite);
			if (filename == "star.png")
			{
				AllStar.pushBack(sprite);
			}
			else AllHeart.pushBack(sprite);
		}
	}
}
void MultiGameScene::SupplyHeartStar(float t);
*/



void MultiGameScene::Get(float t)
{
	HttpRequest* request = new HttpRequest();//生成一个请求对象 
	request->setUrl("https://www.baidu.com/");//设置request访问的URL 
	request->setRequestType(HttpRequest::Type::GET);//设置为get通信方式 
	//char buffer[256];
	//request->setRequestData(buffer, 256);
	request->setResponseCallback(this, httpresponse_selector(MultiGameScene::onHttpRequestCompleted));
	request->setTag("GET test1");
	HttpClient::getInstance()->send(request);//发送请求 
	request->release();//释放 

}
void MultiGameScene::Post(float t)
{
	HttpRequest* request = new HttpRequest();
	request->setUrl("http://httpbin.org/ip");
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(this, httpresponse_selector(MultiGameScene::onHttpRequestCompleted));

	// write the post data
	const char* postData = "visitor=cocos2d&TestSuite=Extensions Test/NetworkTest";
	request->setRequestData(postData, strlen(postData));

	request->setTag("POST test1");
	HttpClient::getInstance()->send(request);
	request->release();
}
void MultiGameScene::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)//回调函数 
{
	if (!response)
	{
		return;
	}

	// You can get original request type from: response->request->reqType
	if (0 != strlen(response->getHttpRequest()->getTag())) //获取请求的标签 
	{
		log("%s completed", response->getHttpRequest()->getTag());
	}
	
	int statusCode = response->getResponseCode();//获取请求的状态码。状态码为200表示请求成功
	char statusString[64] = {};
	sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
	//_labelStatusCode->setString(statusString);
	log("response code: %d", statusCode);
	
	if (!response->isSucceed())
	{
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}

	// dump data
	std::vector<char> *buffer = response->getResponseData();
	/*std::string buf(buffer->begin(),buffer->end());
	log("Http Test, dump data:%s",buf.c_str());
	char* responseString = new char[buffer->size() + 1];
	std::copy(buffer->begin(), buffer->end(), responseString);
	log("Http Test, dump data:%s",responseString);*/
	std::stringstream oss;
	for (unsigned int i = 0; i < buffer->size(); i++) {
		oss << (*buffer)[i];
	}
	std::string temp = oss.str();
	const char * buf = temp.c_str();
	log("Http Test, dump data:%s", buf);
}