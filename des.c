
#include<stdio.h>
#include"shmfifo.h"
int main()
{
	shmfifo* sh =shmfifo_init(1234,0, 0);
	shmfifo_destroy(sh);
	printf("head destory!\n");

	return 0;
}
