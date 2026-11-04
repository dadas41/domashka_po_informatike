#include <sys/sem.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
	int n = atoi(argv[1]);

	int id = semget(IPC_PRIVATE, 1, 0600);
	
	struct sembuf ops = {0, n, 0};
	semop(id, &ops, 1);

	for (int i=0; i<n; i++){
		int f = fork();
		if (f == 0){
			write(1, "ready\n", 6);
			struct sembuf ops1 = {0, -1, 0};
			semop(id, &ops1, 1);

			struct sembuf ops2 = {0, 0, 0};
			semop(id, &ops2, 1);
			write(1, "run\n", 4);

			return 0;
		}
	}

	for (int i=0; i<n; i++){
		wait(0);
	}
	return 0;

}
