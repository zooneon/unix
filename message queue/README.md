## message passing

- 커널 메모리 영역에 채널을 만들어서 프로세스들 사이에 메시지 형태로 정보를 send/receive하는 방법

```c
#include<sys/msg.h>
int msgget(key_t key, int permflags);
//key: queue의 key값
//permflags: queue에 대한 access permission
```

```c
#include<sys/msg.h>
int msgsnd(int mqid, const void *message, size_t size, int flags);
//mqid: queue id
//message: 보낼 message가 저장된 주소
//size: message의 크기
//flags: 0 - 성공할 때까지 blocking
//       IPC_NOWAIT - send가 불가능하면(queue가 가득 찬 경우) 즉시 return
```

```c
#include<sys/msg.h>
int msgrcv(int mqid, void *message, size_t size, long msg_type, int flags);
//mqid: queue id
//message: 받은 message를 저장할 장소의 주소
//size: 저장 장소의 크기
//msg_type: 어떤 메세지를 받을지 선택
//flags: 0 - 성공할 때까지 blocking
//       IPC_NOWAIT - receive가 불가능하면(queue에 해당 메세지가 없는 경우) 즉시 return
//       MSGNOERROR - message가 size보다 길면 초과분을 자름
```

- 커널에서 데이터 통신을 컨트롤할 수 있어 별도의 동기화 로직이 없어도 된다.
    - flag를 설정하지 않을 경우 자동으로 blocking read, blocking write가 가능
- 커널을 통해 데이터를 주고받기 때문에 shared memory 모델보다 느리다.
- message queue 이름처럼 선입선출로 데이터를 처리한다.
- 파일을 이용하는 fifo와 달리 메모리 공간을 이용한다.
- message queue는 큐 하나로 여러 프로세스와 통신 가능하다.
    - 하지만 데이터에 번호를 붙임으로써(mtype) 메세지를 구분한다.
    - 예시를 보면 더 이해가 잘 될 것!

### 예시

- client

```c
//주고받을 메세지 구조, 송신자와 수신자 동일한 구조 사용
struct q_entry{
	//mtype 값은 0 또는 음수이면 안됨, id라 생각하면 편함
	long mtype;
	int data;
};

int main(int argc, char** argv){
	int i, qid, in, id;
	key_t key;
	struct q_entry msg;

	id=atoi(argv[1]);

	key=ftok("key", 3);
	//message queue 생성
	qid=msgget(key, IPC_CREAT|0600);

	for(i=0; i<5; i++){
		scanf("%d", &in);
		//mtype 값 설정
		msg.mtype=id;
		msg.data=in;
		//message 전송, flag가 0이므로 blocking write
		msgsnd(qid, &msg, sizeof(int), 0);
		//현재 mtype+3 메세지를 수신
		msgrcv(qid, &msg, sizeof(int), id+3, 0);
		printf("%d\n", msg.data);
	}

	return 0;
}
```

- server

```c
//주고받을 메세지 구조, 송신자와 수신자 동일한 구조 사용
struct q_entry{
	//mtype 값은 0 또는 음수이면 안됨, id라 생각하면 편함
	long mtype;
	int data;
};

int main(void){
	int i, qid;
	key_t key;
	struct q_entry msg;

	key=ftok("key", 3);
	//message queue 생성
	qid=msgget(key, IPC_CREAT|0600);

	for(i=0; i<15; i++){
		//mtype 값이 절대값보다 작거나 같은 message 수신(여기서는 1~3)
		msgrcv(qid, &msg, sizeof(int), -3, 0);
		//들어온 message의 mtype+3 하여 특정 client와 통신
		msg.mtype+=3;
		msg.data+=8;
		//message 전송, flag가 0이므로 blocking write
		msgsnd(qid, &msg, sizeof(int), 0);
	}

	return 0;
}
```
