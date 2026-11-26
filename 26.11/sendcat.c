#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/wait.h>


void copy(int fd1, int fd2){
	char buf[1024];
	while (1) {
		int s = sendfile(fd2, fd1, 0, 1024);
		if (s == -1) {
			perror("sendfile");
		}
		if (s == 0){
			return;
		}
	}


}

int main(int argc, char* argv[]){
	for (int i=1; i<argc; i++){	
		int fd = open(argv[1], O_RDONLY);
		if (fd < 0) {
			perror("open");
			exit(-1);

		}
		copy(fd, 1);
		close(fd);
	}
	
	return 0;

}
