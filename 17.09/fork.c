#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]){
	for (int i=0; i<argc; i++) {
		fflush(0);
		int f = fork();
		if (f == 0) {//child
			printf("Child ");
			printf("PID: %d ", getpid());
			printf("PPID: %d\n", getppid());
			return 0;
		}
	}
	printf("Parent ");
	printf("PID: %d ", getpid());
        printf("PPID: %d\n", getppid());
	
	return 0;
}
