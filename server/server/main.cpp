#include "ODsocket.h"
#include<pthread.h>
#include<windows.h>
#include<list>
#include<iostream>
#include<string>
#include<cstring>
#include<vector>
#pragma comment(lib, "pthreadVC2.lib")
vector<ODsocket>AllClient;//保存所有客户端
list<string>AllMessage;//保存接收到的所有信息
using namespace std;
void* getMessage(void * ptr)
{
	ODsocket* nowsock = (ODsocket*)ptr;//ptr负责服务器与当前客户端的通信
	char msg[1024];
	int count = 0;
	while (1)
	{
		if (nowsock == NULL || nowsock->m_sock == NULL || nowsock->m_sock <= 0)//如果客户端断开连接，停止该进程
		{
			cout<<"client close";
			break;
		}
		memset(msg, 0, sizeof(msg));
		nowsock->Recv(msg, sizeof(msg));
		string msgstr = msg;
		AllMessage.push_back(msgstr);
	}
	return NULL;
}

void * DoSocket(void * ptr)//服务端向所有客户端广播
{
	std::cout << "广播线程已经启动" << endl;
	int count = 0;
	while (1)
	{
		if (AllClient.size()&&AllMessage.size())
		{
		
			for (auto nowsock : AllClient)
			{
				if (nowsock.m_sock < 0)
				{
					cout<<"cloent close";
				}
				
				for (list<string>::iterator it=AllMessage.begin();it!=AllMessage.end();++it)
				{
					string nowmsg = *it;
					nowsock.Send((char*)nowmsg.c_str(), nowmsg.length(), 0);//广播
					cout << "服务器第" << ++count << "次广播信息:" << nowmsg << endl;
				}
			}
			AllMessage.clear();
		}
	}
}

int main()
{
	pthread_t get;//接受信息的线程
	pthread_t post;//广播信息的线程
	int ret;
	//AllMessage.reserve(3000);
	ret = pthread_create(&post, NULL, DoSocket, NULL);//用于服务端向所有客户端广播的线程
	if (ret)
	{
		printf("创建广播线程失败\n");
		exit(1);
	}
	//1.创建服务端socket
	ODsocket SockServer;
	SockServer.Init();
	bool res = SockServer.Create(AF_INET, SOCK_STREAM, 0);
	if (!res)
	{
		cout << "创建服务端socket失败" << endl;
	}
	//2.绑定端口
	res = SockServer.Bind(8867);
	if (!res)
	{
		cout << "绑定端口失败" << endl;
	}
	//3.监听
	res = SockServer.Listen();
	if (!res)
	{
		cout << "监听失败" << endl;
	}
	char message[30];//message本身没有意义的，只是用来阻挡return 0；
	memset(message, 0, sizeof(message));
	//4.接收客户端的连接
	while (1)
	{

		ODsocket* SockClient = new ODsocket();//负责与当前客户端的通信
		char *address = new char[20];
		std::cout << "accpt被阻塞" << endl;
		SockServer.Accept(*SockClient, address);//此后SockClient负责服务端与该客户端的通信,address保存客户端地址
		std::cout << "阻塞被解除" << endl;
		//SockClient->Send(message, sizeof(message));//为什么要发送消息？？
		ret = pthread_create(&get, NULL, getMessage, SockClient);//创建接收该客户端发送的消息的线程
		AllClient.push_back(*SockClient);
		
	}
	cin >> message;//阻挡return 0；
	return 0;
}