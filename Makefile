.PHONY	: clean 

all	:  put get des

put : put.o shmfifo.o
	gcc  put.o shmfifo.o -o put

get	:get.o shmfifo.o
	gcc get.o shmfifo.o -o get
	
des: des.o shmfifo.o
	gcc des.o shmfifo.o -o des
%.o	: %.c                  
	gcc -c $^ -o $@ 

clean:
	rm -rf  *.o 

# %.c 表示所有的.c文件
# %.o 表示所有的.o文件
# @代表目标名 #^ 代表依赖文件
# % 代表任意字符
