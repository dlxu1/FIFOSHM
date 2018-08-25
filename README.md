项目简介：
将操作共享内存的接口进行封装，并加入互斥与同步机制。解决了共享内存在使用个过程中因控制不当产生的数据混乱问题。同时保留了共享内存速度快的优点，通过接口封装提高了易用性。

使用说明：

创建或打开fifoshm
shmfifo* shmfifo_init(int key,int blocks, int blksz);
（1）key唯一标识该共享内存。
（2）blocks申请内存块的个数
（3）blksz每一个快的大小，以字节为单位
（4）返回值为一个结构体，在进行数据读写时当入参。

向共享内存写数据
void shmfifo_put(shmfifo *fifo, const void *buf)；
(1)fifo为已经打开的共享内存。
（2）buf为需要向共享内存写入的数据。

取出共享内存数据
void shmfifo_get(shmfifo *fifo, void *buf);
和写入接口使用方法相同

销毁共享内存 
void shmfifo_destroy(shmfifo *fifo);


