//исходный код pcat.c лежит в папке 01.10
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>

typedef struct {
	char data[1024];
	int count;
} buffer;

int produser(int fd, buffer* buf, sem_t *full, sem_t *empty){
	while (1){
		sem_wait(full);
	
		buf->count = read(fd, buf->data, 1024);
		if (buf->count < 0) {
			perror("read");
			exit(-1);
		}

		sem_post(empty);
		
		if (buf->count == 0) break;	
	}
}


void consumer(buffer* buf, sem_t *full, sem_t *empty){
	while (1){
		sem_wait(empty);
		
		if (buf->count == 0) break;

		int w = write(1, buf->data, buf->count);
		if (w < 0){
			perror("write");
			exit(-1);
		}
	
		sem_post(full);

	}
}


int main(int argc, char* argv[]){
	buffer* buf = mmap(NULL, sizeof(buffer), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
	
	sem_t* full = sem_open("/full", O_CREAT, 0644, 1);
	sem_t* empty = sem_open("/empty", O_CREAT, 0644, 0);

	int fd = 0;
	if (argc >1){
		fd = open(argv[1], O_RDONLY);
	}
	if (fd < 0){
		perror("open");
		exit(-1);
	}

	int p = fork();
	if (p < 0) {
		perror("fork");
		exit(-1);
	}

	if (p == 0){
		produser(fd, buf, full, empty);
		if (fd != 0) close(fd);
		return 0;
	}

	consumer(buf, full, empty);

	wait(0);

	sem_close(full);
	sem_close(empty);
	sem_unlink("/full");
	sem_unlink("/empty");
	munmap(buf, sizeof(buffer));

	return 0;

		
}
