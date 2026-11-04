#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
	int n = atoi(argv[1]);

	int flag_sem = 0;
	flag_sem = atoi(argv[2]);
	
	if (!flag_sem){
		for (int i=0; i<n; i++){
			int f = fork();
			if (f == 0){
				write(1, "PID", 4);
				write(1, "Hello", 6);
				write(1, "\n", 2);
				return 0;
			}
		}
		for (int i=0; i<n; i++){
			wait(0);
		}
		return 0;
	}
	sem_t *sem = sem_open("/empty", O_CREAT, 0600, 1);

	for (int i=0; i<n; i++){
		int f = fork();
		if (f == 0){
			sem_wait(sem);
			write(1, "PID", 4);
			write(1, "Hello", 6);
			write(1, "\n", 2);

			sem_post(sem);

			return 0;
		}
	}
	for (int i=0; i<n; i++){
		wait(0);
	}
	return 0;

	
}
