#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<string.h>
#include<sys/shm.h>

#ifndef SHMFIFo_H_
#define SHMFIFo_H_


typedef struct shem_head
{
	int wr_idx;  // 读位置
	int rd_idx;  // 写位置
	int blocks;  // 申请空间的总快数
	int blksz;   // 每一块的大小
}shm_head;


typedef struct shmfifo
{
	shm_head* p_head;
	char * p_payload;  //数据存储有效起始位置
	int shmid;
	int sem_mutex;  // 互斥量
	int sem_empty;	 
	int sem_full;  
}shmfifo;

// 初始化
shmfifo* shmfifo_init(int key,int blocks, int blksz);

// 向队列写数据
void shmfifo_put(shmfifo *fifo, const void *buf);

// 取得数据
void shmfifo_get(shmfifo *fifo, void *buf);

// 销毁 
void shmfifo_destroy(shmfifo *fifo);


#endif // SHMFIFO_H_
