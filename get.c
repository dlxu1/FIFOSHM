#include<stdio.h>
#include"shmfifo.h"
int main()
{
	shmfifo* sh =shmfifo_init(1234,0, 0);
	int i=0;
	int *num=malloc(sizeof(int));
	for(;i<15;i++)
	{
		shmfifo_get( sh,num);
		printf("%d \n",*num);
		sleep(1.5);
	}
	return 0;
}
