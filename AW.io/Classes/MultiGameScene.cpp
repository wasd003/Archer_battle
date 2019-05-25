#include "MultiGameScene.h"
#include "HelloWorldScene.h"
#include"ODsocket.h"
#include<pthread.h>
using namespace std;
#pragma comment(lib, "pthreadVC2.lib")
USING_NS_CC;
static MultiGameScene* now;
bool MultiGameScene::init()
{
	if (!Layer::init())return false;
	//this->schedule(schedule_selector(MultiGameScene::Get, this));
	//this->schedule(schedule_selector(MultiGameSc                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    ene::Post, this));
	
	auto label = LabelTTF::create("HelloWorld", "Arial", 24);
	label->setTag(111);
	label->setPosition(Vec2(480, 240));
	addChild(label, 1);
	//连接服务器
	now = this;
	sock_client = new ODsocket();
	sock_client->Init();
	bool res = sock_client->Create(AF_INET, SOCK_STREAM, 0);
	res = sock_client->Connect("192.168.1.103", 8867);
	//向服务器发送信息
	this->postMessage();
	if (res)
	{
		pthread_t tid;
		bool ok = pthread_create(&tid, NULL, MultiGameScene::getMessage, NULL);//创建接受信息的线程
	}



	this->scheduleUpdate();
	return true;
}
Scene* MultiGameScene::CreateScene()
{
	auto scene = Scene::create();
	auto layer = MultiGameScene::create();
	scene->addChild(layer);
	return scene;
}
void MultiGameScene::postMessage()
{
	MessageToPost = "Test";
	sock_client->Send((char*)MessageToPost.c_str(), MessageToPost.length(), 0);
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
void MultiGameScene::update(float t)
{
	LabelTTF* label = (LabelTTF*)getChildByTag(111);
	label->setString(now->strmsg.c_str());
}
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