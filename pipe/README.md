## pipe

- 한 프로세스에서 다른 프로세스로의 단방향 통신 채널이다.
- read, write 명령으로 데이터를 주고 받는다.

```c
#include<unistd.h>
int pipe(int fildes[2]);
//하나의 descriptor로 동시에 읽기/쓰기 할 수 없다.
//filedes[0]: 읽기용
//filedes[1]: 쓰기용
```

- 즉, 양방향 통신을 위해서는 2개의 pipe가 필요하다.
- 데이터를 FIFO(선입선출)로 처리한다.
- 부모-자식 프로세스간 통신에 사용된다.
    - pipe는 `fork()` 명령에 의해 상속 가능하다.
- 시스템은 pipe를 파일처럼 관리하기 때문에 만들 수 있는 pipe의 수가 제한되어 있다.
    - process당 open할 수 있는 파일의 수와 시스템 내의 파일 수는 제한되어 있다.(파일 테이블은 배열이기 때문에)
- 기본적으로 blocking을 지원하기 때문에 특별한 동기화가 필요하지 않다.
    - read가 blocking 되는 경우 : pipe가 비어 있는 경우(pipe에 데이터가 들어오면 깨어난다.)
    - write가 blocking 되는 경우 : pipe가 가득 찬 경우(pipe가 빌 때까지 block)
- 간단하게 사용할 수 있다는 장점이 있다.

### 예시

- 단방향 통신

```c
int main() {
	char ch[10];
	int pid, p[2];
	
	//pipe 만들기에 실패했을 경우
	if(pipe(p) == -1) {
		perror("pipe call");
		exit(1);
	}
	
	//자식 프로세스 생성
	pid = fork();
	//자식 프로세스일 경우
	if(pid == 0) {
		close(p[1]);  //읽기만 가능
		read(p[0], ch, 10);  //blocking read, 데이터가 들어올 대까지 block
		printf("%s\n", ch);
		exit(0);
	}

	close(p[0]);  //쓰기만 가능
	scanf("%s", ch);
	write(p[1], ch, 10);  //blocking write, pipe가 가득 차있으면 block
	wait(0);
	exit(0);
}
```

- 양방향 통신

```c
int main() {
	int i, in, pid, p[2][2];

	//pipe 생성, 양방향 통신이기 때문에 2개 필요
	for(i=0; i<2; i++) {
		pipe(p[i]);

	pid = fork();
	//자식 프로세스일 경우
	if(pid == 0) {
		//pipe는 fork()에 의해 상속되기 때문에 따로 생성하지 않아도 됨
		close(p[0][1]);  //첫 번째 pipe의 쓰기 닫음
		close(p[1][0]);  //두 번째 pipe의 읽기 닫음
		read(p[0][0], &in, sizeof(int));  //blocking read, 데이터가 들어올 대까지 block
		in++;
		write(p[1][1], &in, sizeof(int)); //다시 부모 프로세스에 전송
		exit(0);
	}
	//부모 프로세스일 경우
	close(p[0][0]);  //첫 번째 pipe의 읽기 닫음
	close(p[1][1]);  //두 번째 pipe의 쓰기 닫음
	scanf("%d", &in);
	write(p[0][1], &in, sizeof(int));  //blocking write, pipe가 가득 차있으면 block
	read(p[1][0], &in, sizeof(int));  //자식 프로세스로부터 다시 읽어옴
	printf("%d\n", in);
	wait(0);
	exit(0);
}
```
