#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "protocol.h"
#include <string.h>
#include <map>
using namespace std;

map<int, int> onlineUser;

void* pthread_function1(void* arg) {
	HEAD head = { 0 };
	USER user = { 0 };
	CHATMSG msg = { 0 };
	LOGINBACK back = { 0 };

	char resbuf[200] = { 0 };

	int fd = *((int*)arg);
	while (1) {
		int res = read(fd, resbuf, sizeof(HEAD));
		if (res > 0) {
			memcpy(&head, resbuf, sizeof(HEAD));
			bzero(resbuf, sizeof(resbuf));

			if (head.bussinessType == 1) {// login
				res = read(fd, resbuf, head.bussinessLength);
				memcpy(&user, resbuf, head.bussinessLength);

				cout << "user.name：" << user.name << endl;
				cout << "user.name：" << user.name << endl;

				// 验证账号密码，目前省略
				// 默认成功
				// 添加在线用户
				onlineUser[user.name] = fd;
				
				cout << "onLineUser.size = " << onlineUser.size() << endl;
				
				// 返回结果给客户端
				bzero(&head, sizeof(head));
				bzero(resbuf, sizeof(resbuf));

				back.flag = 1;
				sprintf(back.message, "%s", "登陆成功");

				head.bussinessType = 2;
				head.bussinessLength = sizeof(LOGINBACK);

				memcpy(resbuf, &head, sizeof(head));
				memcpy(resbuf + sizeof(head), &back, sizeof(back));

				res = write(fd, resbuf, sizeof(head) + sizeof(back));
				cout << "服务器发送登录返回res = " << res << endl;

			}
			else if (head.bussinessType == 3) {// chat
				res = read(fd, resbuf, head.bussinessLength);
				memcpy(&msg, resbuf, head.bussinessLength);

				cout << "msg.sendid：" << msg.sendid << endl;
				cout << "msg.recvid：" << msg.recvid << endl;
				cout << "msg.context：" << msg.context << endl;

				bzero(&head, sizeof(head));
				bzero(resbuf, sizeof(resbuf));
				head.bussinessType = 4;
				head.bussinessLength = sizeof(msg);
				memcpy(resbuf, &head, sizeof(head));
				memcpy(resbuf + sizeof(head), &msg, sizeof(msg));

				// 判断是否在线
				// 如果不在线先保存聊天记录
				
				// 发送给发送者
				res = write(onlineUser[msg.sendid], resbuf, sizeof(head) + sizeof(msg));
				cout << "服务器发送给发送者返回res = " << res << endl;

				// 发送给接收者
				res = write(onlineUser[msg.recvid], resbuf, sizeof(head) + sizeof(msg));
				cout << "服务器发送给接收者返回res = " << res << endl;
				
			}
		}
	}
	return NULL;
}

int main() {
	int res = 0;
	char resbuf[100] = { 0 };
	struct sockaddr_in addr;
	int len = 0;

	int acceptfd = 0;
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd < 0) {
		perror("socket error");
	}
	else {
		cout << "网络初始化成功 socketfd = " << socketfd << endl;
		// 网络协议族（ipv4/6） 
		addr.sin_family = AF_INET;
		// 网络端口 
		addr.sin_port = htons(10001);
		// 服务器网络IP地址，写服务器自己的IP 
		// INADDR_ANY由系统默认提供 
		addr.sin_addr.s_addr = INADDR_ANY;
		len = sizeof(addr);
		if (bind(socketfd, (struct sockaddr*)&addr, len) == -1) {
			perror("bind error");
			return -1;
		}
		// 设置网络通道的监听方式 
		// 后期优化 
		if (listen(socketfd, 10) == -1) {
			perror("listen error");
			return -1;
		}
		cout << "服务器网络搭建成功" << endl;
		while (1) {
			// accept阻塞时函数 
			// acceptfd代表连接成功的客户端 
			acceptfd = accept(socketfd, NULL, NULL);
			cout << "有客户端连接访问acceptfd = " << acceptfd << endl;
			pthread_t thread1;
			pthread_create(&thread1, NULL, pthread_function1, &acceptfd);
			/*pid_t pid = fork();
			if (pid == 0) {
				while (1) {
					res = read(acceptfd, resbuf, sizeof(resbuf));
					cout << "服务器收到acceptfd" << acceptfd << " res = " << res << " resbuf = " << resbuf << endl;
				}
			}*/
		}
	}
	return 0;
}