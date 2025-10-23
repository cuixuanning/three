#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "protocol.h"
#include <string.h>
using namespace std;

int isLogin = true;
int loginUser = 0;

void* readthread_fun(void* arg) {
	char buf[200] = { 0 };
	HEAD head = { 0 };
	LOGINBACK back = { 0 };
	CHATMSG msg = { 0 };
	int fd = *((int*)arg);

	while (1) {
		bzero(buf, sizeof(buf));
		int res = read(fd, buf, sizeof(HEAD));
		if (res > 0) {
			memcpy(&head, buf, sizeof(head));
			bzero(buf, sizeof(buf));

			if (head.bussinessType == 2) {// login
				res = read(fd, buf, head.bussinessLength);
				memcpy(&back, buf, head.bussinessLength);

				cout << "back.flag = " << back.flag << endl;
				if (back.flag == 1) {
					cout << "back.message = " << back.message << endl;
					isLogin = false;
				}
			}
			else if (head.bussinessType == 4) {// chat
				res = read(fd, buf, head.bussinessLength);
				memcpy(&msg, buf, head.bussinessLength);

				if (loginUser == msg.sendid) {
					cout << "我说：" << msg.context << endl;
				}
				else if (loginUser == msg.recvid) {
					cout << msg.sendid << "对我说：" << msg.context << endl;
				}
			}
		}
	}



	return NULL;
}

void* writethread_fun(void* arg) {
	int res = 0;

	char buf[200] = { 0 };

	HEAD head = { 0 };
	USER user = { 0 };
	CHATMSG mgs = { 0 };

	int fd = *((int*)arg);
	while (1) {
		bzero(buf, sizeof(buf));
		if (isLogin == true) {
			cout << "请输入账号：";
			cin >> user.name;
			cout << "请输入密码：";
			cin >> user.pass;

			loginUser = user.name;

			head.bussinessType = 1;
			head.bussinessLength = sizeof(user);

			memcpy(buf, &head, sizeof(head));
			memcpy(buf + sizeof(head), &user, sizeof(user));

			res = write(fd, buf, sizeof(head) + sizeof(user));
			cout << "客户端发送res = " << res << endl;
		}
		else {
			cout << "客户进入聊天" << endl;

			cout << "请输入聊天账号";
			cin >> mgs.recvid;
			cout << "请输入聊天内容";
			cin >> mgs.context;
			 
			mgs.sendid = loginUser;

			head.bussinessType = 3;
			head.bussinessLength = sizeof(mgs);

			memcpy(buf, &head, sizeof(head));
			memcpy(buf + sizeof(head), &mgs, sizeof(mgs));

			res = write(fd, buf, sizeof(head) + sizeof(mgs));
			cout << "客户端发送res = " << res << endl;


		}
		// 因为本地线程执行非常快，但客户端发出需要经过
		// 服务器接受->业务分析->业务执行->服务器返回
		// 所以需要一个延时，让逻辑在读线程读到登陆结果之后
		sleep(3);
	}
	return NULL;
}

int main() {
	
	pthread_t readthread;
	pthread_t writethread;


	int res = 0;
	char buf[100] = { 0 };
	int len = 0;
	struct sockaddr_in addr;
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);

	if (socket < 0) {
		perror("socket error");
		return -1;
	}
	else {
		cout << "客户端网络初始化成功 socketfd = " << socketfd << endl;

		// 网络协议族
		addr.sin_family = AF_INET;
		addr.sin_port = htons(10001);
		addr.sin_addr.s_addr = inet_addr("192.168.177.129");
		len = sizeof(addr);

		if (connect(socketfd, (struct sockaddr*)(&addr), len) == -1) {
			perror("connect error");
			return -1;
		}
		cout << "客户端连接服务器成功" << endl;


		pthread_create(&readthread, NULL, readthread_fun, &socketfd);
		pthread_create(&writethread, NULL, writethread_fun, &socketfd);

		while (1) {

		}

	}
}