#include "OP1.h"
#include "OP2.h"
#include "OP3.h"
#include <stdio.h>
int main(int argc, char ** argv){
	int eg = 0;
	printf("1.simple demo\n");
	printf("2.3d graph algorithm\n");
	printf("3.frameBuffer object\n");
	printf("please input a number to choose demo:");
#ifdef WIN32
	scanf_s("%d",&eg);
#else
    scanf("%d",&eg);
#endif
	switch (eg)
	{
	case 1:
		return OP1::run();
	case 2:
		return OP2::run();
	case 3:
		return OP3::run();
	default:
		return OP2::run();
		break;
	}
	
}