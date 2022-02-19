## record locking

- 파일의 일부를 순차적으로 하나의 프로세스만 읽어가도록 하여 동기화 진행
    - critical section 구현
- locking : 특정 record에 대한 다른 프로세스의 읽기/쓰기 제한
    - read lock : 읽기 허용, 쓰기 제한
    - write lock : 읽기, 쓰기 모두 제한
    - unlocking : 제한 해제

```c
#include<fcntl.h>
int fcntl(int filedes, int cmd, struct flock *ldata);
//filedes: lock을 설정하려는 file의 descriptor
//cmd: F_GETLK - lock 정보 얻기
//     F_SETLK: non-blocking locking or unlocking
//     F_SETLKW: blocking locking(일반적인 locking)
//ldata: lock을 걸 위치 지정
```

- lock 정보는 fork()에 의해 계승되지 않음
- 모든 lock은 프로세스 종료시 자동으로 unlcok
- locking 순서에 따라 데드락이 발생할 수 있음
    - F_SETLKW 명령을 통해 데드락이 발생하는지 검사할 수 있음(가능성이 있으면 -1 return)

### 예시

```c
int main() {
	int fd, i, num;
	struct flock lock;

	fd = open("data1", O_RDWR|O_CREAT, 0600);

	//lock 위치 지정
	lock.l_whence = SEEK_CUR;
	lock.l_len = sizeof(int);

	for(i=0; i<10; i++) {
		//쓰기 lock 설정
		lock.l_type = F_WRLCK;
		lock.l_start = 0;
		fcntl(fd, F_SETLKW, &lock);

		read(fd, &num, sizeof(int));
		num = num + 10;
		sleep(1);
		lseek(fd, -sizeof(int), SEEK_CUR);
		wrtie(fd, &num, sizeof(int));
		//lock 해제
		lock.l_type = F_UNLCK;
		//lock이 걸려 있는 위치로 돌아가야함
		lock.l_start = -sizeof(int);
		fcntl(fd, F_SETLK, &lock);

	}

	lseek(fd, 0, SEEK_SET);
	for(i=0; i<10; i++) {
		read(fd, &num, sizeof(int));
		printf(”%d\n”, num);
	}

	return 0;
}
```
