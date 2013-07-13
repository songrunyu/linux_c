#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
int main()      
{
	pid_t result;
	int r_num;
	int pipe_fd1[2];
	int pipe_fd2[2];
	char buf_r[100];

	if(pipe(pipe_fd1)<0) {
	      	perror("pipe");		
		exit(EXIT_FAILURE);	
	}
	if(pipe(pipe_fd2)<0) {
	      	perror("pipe");		
		exit(EXIT_FAILURE);	
	}

	result=fork();
	if(result<0) {
	      	perror("fork");		
		exit(EXIT_FAILURE);	
	}
	else if(result==0) {//子
	      	close(pipe_fd1[1]); 
		close(pipe_fd2[0]);

		//向pipe1写
		char * buf_w = "child process!\n";
		write(pipe_fd2[1],buf_w,strlen(buf_w)+1);
		printf("child process has written %s\n",buf_w);
		//从pipe2读
		memset(buf_r,0,sizeof(buf_r));
		read(pipe_fd1[0],buf_r,100);
		printf("child process read from pipe:%s",buf_r);

                   	exit(0);
	}
	else {//父
		close(pipe_fd1[0]);
		close(pipe_fd2[1]);
		
		char *buf_w = "parent process!\n";
		write(pipe_fd1[1],buf_w,strlen(buf_w)+1);
		printf("parent process has written:%s\n",buf_w);
		memset(buf_r,0,sizeof(buf_r));
		read(pipe_fd2[0],buf_r, 100);
		printf("parent process read from pipe:%s\n",buf_r);

	      	close(pipe_fd1[1]); 
		close(pipe_fd2[0]);

		waitpid(result,NULL,0);
		exit(0);	
	}
}

