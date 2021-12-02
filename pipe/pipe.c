#include<sys/stat.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int main() {
	int i, in, pid, p[2][2];

	for (i = 0; i < 2; i++)
		pipe(p[i]);

	pid = fork();
	if (pid == 0) {
		close(p[0][1]);
		close(p[1][0]);

		read(p[0][0], &in, sizeof(int));
		in++;
		write(p[1][1], &in, sizeof(int));

		exit(0);
	}

	close(p[0][0]);
	close(p[1][1]);

	scanf("%d", &in);
	
	write(p[0][1], &in, sizeof(int));
	read(p[1][0], &in, sizeof(int));

	printf("%d\n", in);

	wait(0);
	exit(0);
}
