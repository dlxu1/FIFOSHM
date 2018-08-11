#include<stdio.h>
#include"shmfifo.h"

int main()
{
	shmfifo* sh =shmfifo_init(1234,20,4);
	int i=0;
	for(;i<10;i++)
	{
		shmfifo_put(sh, &i);
		printf("%d\n",i);
		sleep(1);
	}
	return 0;
}
