#include<unistd.h>
#include<stdlib.h>

int main(int argc, char **argv) {
        rmdir(argv[1]);
        exit(0);
}
