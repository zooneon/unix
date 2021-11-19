#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>

int main() {
	int fd, i, n;
	char buf[512];

	if((fd=open("fifo", O_WRONLY|O_NONBLOCK)) < 0) {
		printf("fifo open failed");
		exit(1);
	}

	for(i=0; i<3; i++) {
		n = read(0, buf, 512);
		write(fd, buf, n);
	}

	exit(0);
}

