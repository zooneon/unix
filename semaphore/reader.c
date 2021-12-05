#include<sys/mman.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

union semun{
	int val;
	struct semid_ds *buf;
	ushort *array;
};

int main(void){
	int fd, i, semid, *addr;
	key_t key;
	union semun arg;
	struct sembuf p_buf;

	key=ftok("key", 1);
	semid=semget(key, 1, 0600|IPC_CREAT|IPC_EXCL);
	if(semid==-1){
		semid=semget(key, 1, 0);
	}
	else{
		arg.val=0;
		semctl(semid, 0, SETVAL, arg);
	}

	fd=open("data", O_RDWR|O_CREAT, 0600);
	addr=mmap(NULL, 512, PROT_READ|PROT_WRITE, MAP_SHARED, fd , 0);
	ftruncate(fd, 10*sizeof(int));

	p_buf.sem_num=0;
	p_buf.sem_op=1;
	p_buf.sem_flg=0;

	for(i=0; i<10; i++){
		scanf("%d", addr+i);
		semop(semid, &p_buf, 1);
	}

	exit(0);
}
