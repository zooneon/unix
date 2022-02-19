## fifo

- fifo라는 파일을 이용하여 통신한다.
    - 실제로 파일 이름을 부여 받음
    - 소유자, 크기, permission을 갖음
    - 일반 파일처럼 open, close, read, write 가능
- pipe는 동일한 ancestor를 갖는 프로세스들만 연결 가능하지만 fifo는 모든 프로세스들을 연결 가능하다.
    - fifo 파일을 별도로 생성하여 사용하기 때문에
    - 좀 더 넓은 의미의 pipe

```c
#include<sys/types.h>
#include<sys/stat.h>
int mkfifo(const char *pathname, mode_t mode);
//pathname: 파일 이름
//mode: permission 정보
```

- open 호출은 다른 프로세스가 읽기 또는 쓰기를 위해 open될 때까지 blocking
    - 동시에 읽기 fifo를 open하면 둘 다 block되므로 데드락 발생
    - non-blocking open의 경우 상대 프로세스가 준비되지 않으면 -1을 return
- pipe와 마찬가지로 blocking read, blocking write을 지원한다.

### 예시

- Reader

```c
int main() {
	int fd, n;
	char buf[512];
	//fifo 생성
	mkfifo("fifo", 0600);
	//일반 파일처럼 open
	fd = open("fifo", O_RDWR);
	for( ; ; ) {
		n = read(fd, buf, 512);  //blocking read, 데이터가 들어올 대까지 block
		write(1, buf, n);
	}
}
```

- Writer

```c
int main() {
	int fd, i, nread;
	char buf[512];
	//fifo를 open하지 않아 실패한 경우	
	if((fd=open("fifo", O_WRONLY|O_NONBLOCK)) < 0) {
		printf("fifo open failed");
		exit(1);
	}

	for(i=0; i<3; i++) {
		nread = read(0, buf, 512);
		write(fd, buf, nread);  //blocking write, pipe가 가득 차있으면 block
	}

	exit(0);
}
```
