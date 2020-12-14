#pragma once
#include <windef.h>
typedef struct rgstdUsr {
	char userName[10];
	BYTE ipFild[4];
	int port;
	BOOL onLStat;
}RgstUsr,*pRgstdUsr;