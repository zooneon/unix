#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc, char **argv) {
	char buf[512];
	int fd1, fd2, n;

	fd1 = open(argv[1], O_RDONLY);
	if(fd1 == -1) {
		exit(0);
	}

	fd2 = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0600);

	while((n=read(fd1, buf, 512)) !== 0) {
		write(fd2, buf, n);
	}

	exit(0);
}
