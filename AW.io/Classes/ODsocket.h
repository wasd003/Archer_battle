#pragma once
#include<winsock.h>
#include<string>
typedef int socklen_t;
//#include<iostram>
using namespace std;
class ODsocket
{
public:
	ODsocket(SOCKET sock = INVALID_SOCKET);//构造函数

	int SocketID;

	bool Create(int AF, int type, int protocal = 0);

	bool Connect(const string &ip, unsigned short port);

	bool Bind(unsigned short port);

	bool Listen(int backlog = 5);

	bool Accept(ODsocket &s, char *fromip = NULL);

	int Send(char * buffer, int len, int flag = 0);

	int Recv(char *buffer, int len, int flag=0);
	
	int Close();

	int GetError();

	static int Init();

	static int Clean();

	static bool DnsParse(const char *domain, char * ip);//DNS解析

	ODsocket&  operator=(SOCKET s);//重载ODsocket等号运算符

	operator SOCKET();

	SOCKET m_sock;

};

