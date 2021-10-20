#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<stdlib.h>
#include<stdio.h>

int main(int argc, char **argv) {
	DIR *dp;
	struct dirent *d;
	struct stat buf;

	stat(argv[1], &buf);
	if(S_ISREG(buf.st_mode)) {
		remove(argv[1]);
	}
	else {
		dp = opendir(argv[1]);
		chdir(argv[1]);
		d = readdir(dp);
		while(d != NULL) {
			remove(d->d_name);
			d = readdir(dp);
		}

		chdir("..");
		rmdir(argv[1]);
	}

	exit(0);
}
