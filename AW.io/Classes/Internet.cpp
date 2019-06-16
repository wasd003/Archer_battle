#if 0
#include "ODSocket"
USING_NS_CC;
void Internet::connectServer()
{
	// 初始化
	ODSocket socket;
	socket.Init();
	socket.Create(AF_INET, SOCK_STREAM, 0);

	// 设置服务器的IP地址，端口号
	// 并连接服务器 Connect
	const char* ip = "127.0.0.1";
	int port = 12345;
	bool result = socket.Connect(ip, port);

	// 发送数据 Send
	socket.Send("login", 5);

	if (result) {
		CCLOG("connect to server success!");
		// 开启新线程，在子线程中，接收数据
		std::thread recvThread = std::thread(&HelloWorld::receiveData, this);
		recvThread.detach(); // 从主线程分离
	}
	else {
		CCLOG("can not connect to server");
		return;
	}
}
#endif