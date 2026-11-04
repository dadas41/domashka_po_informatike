#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

void copy(int fd1, int fd2){
	char buf[1024];
	while (1) {
		int count = read(fd1, buf, sizeof(buf));
		if (count < 0){
			perror("read");
			exit(-1);
		}
		if (count == 0) break;
		count = write(fd2, buf, count);
		if (count < 0) {
			perror("write");
			exit(-1);
		}
	
	}


}

int main(int argc, char* argv[]){
	int fds[2];
	
	if (pipe(fds) < 0){
		perror("pipe");
		return 0;
	}

	int p = fork();
	if (p == 0){
		for (int i=1; i<argc; i++){
			int fd = open(argv[i], O_RDONLY);
			if (fd < 0) {
				perror("open");
				exit(-1);

			}
			copy(fd, fds[1]);
			close(fd);
		}
		if (argc == 1){
			copy(0, fds[1]);
		
		}
		return 0;
	}

	close(fds[1]);
	copy(fds[0], 1);
	
	wait(0);
	
	close(fds[0]);
	return 0;

}
