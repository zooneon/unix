#include<sys/stat.h>
#include<sys/types.h>

int main(int argc, char **argv) {
        mkdir(argv[1], 0700);
        exit(0);
}
