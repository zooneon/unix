#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>

int main() {
	int fd, n;
	char buf[512];

	mkfifo("fifo", 0600);

	fd = open("fifo", O_RDWR);

	for( ; ; ) {
		n = read(fd, buf, 512);
		write(1, buf, n);
	}
}

