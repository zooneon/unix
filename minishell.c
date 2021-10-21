#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

int main(void){
        char name[100], in[50], *res[20]={0};
        int i, status;
		pid_t pid;

        while (1){
		getcwd(name, 99);
		printf("%s> ", name);

        	gets(in);
		if (in[0]=='\0')
			continue;

		i=0;
        	res[i]=strtok(in, " ");
		while (res[i]){
			res[++i]=strtok(NULL, " ");
		}

        	if (strcmp(res[0], "exit")==0){
        		exit(0);
		}
		else if (strcmp(res[0], "cd_m")==0){
			chdir(res[1]);
		}
		else {
			switch(pid=fork()) {
				case -1:
					perror("fork failed");
					break;
				case 0:
					execvp(res[0], res);
				default:
					wait(&status);
			}
		}
	}

	return 0;
}
