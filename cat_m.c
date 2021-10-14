#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc, char **argv) {
        char buf[512];
        int fd, n;

        fd = open(argv[1], O_RDONLY);
        if(fd == -1) {
                return;
        }

        while((n=read(fd, buf, 512)) != 0) {
                write(1, buf, n);
        }

        exit(0);
}
