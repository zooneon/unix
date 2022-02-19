## shared memory

- 둘 이상의 프로세스가 물리적 메모리의 일부를 공유
    - memory mapping과 달리 파일을 이용하지 않고 메모리를 직접 공유
- 가장 효율적인 IPC 기법
    - 다른 작업들과 달리 생성할 때를 제외하고 OS 호출이 없음
    - 메모리에 직접 접근
    - 하지만 별도의 동기화 기술이 필요함

```c
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
int shmget(key_t key, size_t size, int permflag);
//key: 공유 메모리 영역의 identifier
//size: 공유 메모리 영역의 최소 크기(page 단위)
//permflag: access permission
//공유 메모리 영역의 identifier를(shmid) return
```

- shmat을 호출하여 shmget 호출에 의해 할당된 메모리 영역을 자신의 논리적 자료 공간에 부착한다.

```c
#include<sys/shm.h>
int *shmat(int shmid, const void *daddr, int shmflag);
//shimid: 공유 메모리 identifier
//daddr: 프로세스 내의 부착 위치, NULL인 경우 시스템이 위치 결정
//shmflag: 공유 메모리에 대해 읽기/쓰기 여부를 지정
//process 내의 주소(할당된 shared memory) return, 포인터처럼 사용 가능
```

- shmdt을 호출하여 공유 메모리 영역을 프로세스의 논리적 주소 공간으로부터 떼어낸다.

```c
int shmdt(memptr);
//memptr: 공유 메모리 영역에 대한 유효 주소
```

### 예시

- reader

```c
//동기화를 위해 semaphore 사용
union semun{
	int val;
	struct semid_ds *buf;
	ushort *array;
};

int main(void){
	int shmid, semid, i, n, *buf;
	key_t key1, key2;
	union semun arg;
	struct sembuf p_buf;

	key1=ftok("key", 1);
	semid=semget(key1, 1, 0600|IPC_CREAT|IPC_EXCL);
	if(semid==-1){
		semid=semget(key1, 1, 0);
	}
	else{
		arg.val=0;
		semctl(semid, 0, SETVAL, arg);
	}

	key2=ftok("key", 2);
	//shared memory 생성
	shmid=shmget(key2, 10*sizeof(int), 0600|IPC_CREAT);
	//프로세스 공간에 부착, 포인터처럼 사용 가능
	buf=(int *)shmat(shmid, 0, 0);

	for(i=0; i<10; i++){
		scanf("%d", (buf+i));
		p_buf.sem_num=0;
		p_buf.sem_op=1;
		p_buf.sem_flg=0;
		semop(semid, &p_buf, 1);
	}

	exit(0);
}
```

- writer

```c
//동기화를 위해 semaphore 사용
union semun{
	int val;
	struct semid_ds *buf;
	ushort *array;
};

int main(void){
	int shmid, semid, i, n, *buf;
	key_t key1, key2;
	union semun arg;
	struct sembuf p_buf;

	key1=ftok("key", 1);
	semid=semget(key1, 1, 0600|IPC_CREAT|IPC_EXCL);
	if(semid==-1){
		semid=semget(key1, 1, 0);
	}
	else{
		arg.val=0;
		semctl(semid, 0, SETVAL, arg);
	}

	key2=ftok("key", 2);
	//shared memory 생성
	shmid=shmget(key2, 10*sizeof(int), 0600|IPC_CREAT);
	//프로세스 공간에 부착, 포인터처럼 사용 가능
	buf=(int *)shmat(shmid, 0, 0);

	for(i=0; i<10; i++){
		p_buf.sem_num=0;
        	p_buf.sem_op=-1;
        	p_buf.sem_flg=0;
		semop(semid, &p_buf, 1);
		printf("%d\n", *(buf+i));
	}
	//공유 메모리 제거
	shmctl(shmid, IPC_RMID, 0);
	exit(0);
}
```
