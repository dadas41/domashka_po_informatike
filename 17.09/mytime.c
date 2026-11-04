#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>

int main(int argc, char* argv[]){
	struct timeval tv1;
	struct rusage usage;
	int status;

	int fds[2];
	if (pipe(fds)<0){
		perror("pipe");
		return 0;
	}

	gettimeofday(&tv1, 0);
	int p = fork();
	if (p == 0){
		close(fds[0]);

		dup2(fds[1], 1);
		close(fds[1]);

		execvp(argv[1], &argv[1]); 
		return 0;
	}

	close(fds[1]);
	
	int byte = 0;
	int word = 0;
	int line = 0;

	char buf[4096];
	int n = 0;
	n = read(fds[0], buf, sizeof(buf));
	byte += n;

	for (int i=0; i<n; i++){
		char c = buf[i];
		if (c == '\n') {
			line+=1;
			if (i!= 0 && buf[i-1] != ' ') word+=1;

		}
		else if (c == ' ' && i != 0 && buf[i-1] != ' '){
			word += 1;
			
		}
		else if (i==n-1 && c != ' ') word += 1;
	}

	close(fds[0]);

	wait3(&status, 0, &usage);
	struct timeval tv2;
	gettimeofday(&tv2, 0);
	printf("Time:%6ld.%03lds\n", (tv2.tv_sec-tv1.tv_sec), (tv2.tv_usec-tv1.tv_usec)/1000);
	printf("User:%6ld.%03lds\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec / 1000);
	printf("Sys:%7ld.%03lds\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec / 1000);
	printf("\n");

	printf("Byte: %d\n", byte);
	printf("Word: %d\n", word);
	printf("Line: %d\n", line);
	
	printf("exitstatus = %d\n", WEXITSTATUS(status));
	printf("signalstatus = %d\n", WTERMSIG(status));	
	return 0;	
}
