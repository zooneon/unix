## semaphore

- 프로세스 간 데이터를 동기화하고 보호하는 역할

```c
semwait(sem);
//do something;
semsignal(sem);
```

### 예시

- reader

```c
//semaphore 구조체
union semun{
	int val;  //단일 semaphore 사용시
	struct semid_ds *buf;  //stat 정보를 가져올 때ㅠ
	ushort *array;  //여러 semaphore 사용시
};

int main(){
	int fd, i, semid, *addr;
	key_t key;
	union semun arg;
	struct sembuf p_buf;

	key=ftok("key", 1);
	//semaphore 생성
	semid=semget(key, 1, 0600|IPC_CREAT|IPC_EXCL);
	if(semid==-1){
		semid=semget(key, 1, 0);
	}
	else{
		//semaphore 값을 0으로 초기화
		arg.val=0;
		semctl(semid, 0, SETVAL, arg);
	}

	fd=open("data", O_RDWR|O_CREAT, 0600);
	addr=mmap(NULL, 512, PROT_READ|PROT_WRITE, MAP_SHARED, fd , 0);
	ftruncate(fd, 10*sizeof(int));
	//signal연산
	p_buf.sem_num=0;
	p_buf.sem_op=1;
	p_buf.sem_flg=0;

	for(i=0; i<10; i++){
		scanf("%d", addr+i);
		semop(semid, &p_buf, 1);
	}

	exit(0);
}
```

- writer

```c
//semaphore 구조체
union semun{
	int val;  //단일 semaphore 사용시
	struct semid_ds *buf;  //stat 정보를 가져올 때ㅠ
	ushort *array;  //여러 semaphore 사용시
};

int main(){
	int fd, i, semid, *addr;
	key_t key;
	union semun arg;
	struct sembuf p_buf;

	key=ftok("key", 1);
	//semaphore 생성
	semid=semget(key, 1, 0600|IPC_CREAT|IPC_EXCL);
	if(semid==-1){
		semid=semget(key, 1, 0);
	}
	else{
		//semaphore 값을 0으로 초기화
		arg.val=0;
		semctl(semid, 0, SETVAL, 1);
	}
	fd=open("data", O_RDWR|O_CREAT, 0600);
	//memory mapping 동기화에 사용
	addr=mmap(NULL, 512, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	ftruncate(fd, 10*sizeof(int));
	//wait연산, semaphore 값이 음수가 되면 block
	p_buf.sem_num=0;
	p_buf.sem_op=-1;
	p_buf.sem_flg=0;

	for(i=0; i<10; i++){
		semop(semid, &p_buf, 1);
		printf("%d\n", *(addr+i));
	}
	//semaphore 제거
	semctl(semid, 0, IPC_RMID, 0);
	exit(0);
}
```
