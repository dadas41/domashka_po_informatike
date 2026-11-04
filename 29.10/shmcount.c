#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/shm.h>


int main(int argc, char* argv[]){
	int n = atoi(argv[1]);
	
	int id = shmget(IPC_PRIVATE, 4096, 0666 | IPC_CREAT);
	int* count = shmat(id, NULL, 0);

	for (int i=0; i<n; i++){
		int f = fork();
		if (f == 0){
			for (int j=0; j<1000000; j++) {
				(*count)++;

			}

			return 0;
		}
	}
	for (int i=0; i<n; i++){
		wait(0);
	}

	printf("%d\n", *count);
	return 0;
}
