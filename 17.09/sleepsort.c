#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	for (int i=1; i<argc; i++){
		int f = fork();
		if (f == 0) {
			sleep(atoi(argv[i]));
			printf(" %d ", atoi(argv[i]));
			return 0;	
		}
	
	}
	for (int i=1; i<argc; i++){
		wait(NULL);
	}
	printf("\n");
	return 0;

}
