#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<time.h>

int main(int argc, char **argv) {
	struct stat buf;
	DIR *dp;
	struct dirent *d;

	dp = opendir(".");
	d = readdir(dp);
	while(d != NULL) {
		stat(d->d_name, &buf);
		if(S_ISREG(buf.st_mode)) {
			printf("F 0%o %d %d %d %d %s %s\n", buf.st_mode&0777, buf.st_nlink, buf.st_uid, buf.st_gid, buf.st_size, ctime(&buf.st_mtime), d->d_name);
		}
		else if(S_ISDIR(buf.st_mode)) {
			printf("D 0%o %d %d %d %d %s %s\n", buf.st_mode&0777, buf.st_nlink, buf.st_uid, buf.st_gid, buf.st_size, ctime(&buf.st_mtime), d->d_name);
		}

		d = readdir(dp);
	}

	exit(0);
}
