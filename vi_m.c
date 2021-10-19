#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char **argv) {
	char buf[512];
	int fd, n;

	fd = open(argv[1], O_WRONLY|O_CREAT, 0600);
	while((n=read(fd, buf, 512)) != 0) {
		write(1, buf, n);
	}

	n = read(0, buf, 512);
	buf[n] = '\0';
	while(strcmp("quit\n", buf) != 0) {
		write(fd, buf, n);
		n = read(0, buf, 512);
		buf[n] = '\0';
	}

	exit(0);
}
