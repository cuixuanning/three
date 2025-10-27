#pragma once
typedef struct head {
	int bussinessType;  // 请求体业务类型
	int bussinessLength; // 请求体字节长度
}HEAD;

// 请求体业务
// 登陆业务
typedef struct user {
	int name;
	char pass[20];
}USER;

// 聊天业务，既是请求体也是返回体
typedef struct chatmsg {
	int sendid;
	int recvid;
	char context[150];
}CHATMSG;

// 返回体
// 登陆业务返回体
typedef struct loginback {
	int flag;
	char message[20];
}LOGINBACK;