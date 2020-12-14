#pragma once
typedef struct tagHeader {
	char type;
	int len;
}Header, *pHeader;
#define LOGIN_IO        1//µÇÂ¼
#define SEND_MESSAGE    3//·¢ËÍĞÅÏ¢