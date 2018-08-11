#include"shmfifo.h"

union semun
{
	int val;
};

shmfifo* shmfifo_init(int key,int blocks, int blksz)
{
	shmfifo* sh=(shmfifo*)malloc(sizeof(shmfifo));
	if(sh==NULL)
	{
		perror("malloc filed\n");
		exit(1);
	}
	memset(sh,0x00,sizeof(shmfifo));
	int shmid=shmget(key,0,0);
	if(shmid==-1)
	{
		int shmid= shmget(key,blocks*blksz+sizeof(shm_head),IPC_CREAT | 0644);
		sh->p_head=shmat(shmid,NULL,0); //初始化头部
		sh->p_head->blocks=blocks;
		sh->p_head->blksz=blksz;
		sh->p_head->rd_idx=0;
		sh->p_head->wr_idx=0;

		sh->p_payload=(char*)(sh->p_head+1);
		sh->sem_mutex=semget(key,1,IPC_CREAT | 0644);
		union semun su={1};
		semctl(sh->sem_mutex,0,SETVAL,su);


		sh->sem_empty=semget(key+1,1,IPC_CREAT | 0644);
		su.val= blocks ;
		semctl(sh->sem_empty,0,SETVAL,su);

		sh->sem_full=semget(key+2,1,IPC_CREAT | 0644);
		su.val= 0;
		semctl(sh->sem_full,0,SETVAL,su);

		sh->shmid=shmid;
	}
	else
	{	
		sh->p_head=shmat(shmid,NULL,0);

		sh->p_payload=(char*)(sh->p_head+1);
		sh->sem_mutex=semget(key,0,0);
		sh->sem_empty=semget(key+1,0,0);
		sh->sem_full=semget(key+2,0,0);
		sh->shmid=shmid;
	}
	return sh;
}

static void P(int semid)
{	
	struct sembuf sops[1]={0,-1,0};
	semop(semid,sops,1);
}

static void V(int semid)
{
	struct sembuf sops[1]={0,1,0};
	semop(semid,sops,1);
}
void shmfifo_put(shmfifo *fifo, const void *buf)
{
	P(fifo->sem_empty);
	P(fifo->sem_mutex);

	memcpy(fifo->p_payload+fifo->p_head->wr_idx * fifo->p_head->blksz,buf, fifo->p_head->blksz );
	fifo->p_head->wr_idx=(fifo->p_head->wr_idx+1)%fifo->p_head->blocks;

	V(fifo->sem_mutex);
	V(fifo->sem_full);
}

// 取得数据
void shmfifo_get(shmfifo *fifo, void *buf)
{
	P(fifo->sem_full);
	P(fifo->sem_mutex);

	memcpy(buf,fifo->p_payload+fifo->p_head->rd_idx * fifo->p_head->blksz,fifo->p_head->blksz);
	fifo->p_head->rd_idx=(fifo->p_head->rd_idx+1)%fifo->p_head->blocks;


	V(fifo->sem_mutex);
	V(fifo->sem_empty);
}

// 销毁 
void shmfifo_destroy(shmfifo *fifo)
{
	semctl(fifo->sem_mutex,0,IPC_RMID);
	semctl(fifo->sem_empty,0,IPC_RMID);
	semctl(fifo->sem_full,0,IPC_RMID);
	shmdt(fifo->p_head);
	shmctl(fifo->shmid,IPC_RMID,0);
	free(fifo);
}

