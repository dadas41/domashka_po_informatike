#include <stdio.h>
#include <sys/epoll.h>



#define N 5

void work(int fd1, int fd2){
	




}

int main(int argc, char* argv[]){
	int ep = epoll_create(0);
	if (ep == -1){
		perror("epoll_create");
		return 1;
	}


	for (int i = 0; i< N; i++){
		int fds[2];
		if (pipe(fds) == -1){
			perror("pipe");
			return 1;
		}

		int p = fork();
		if (p == -1){
			perror("fork");
			return 1;
		}

		if (p == 0) {
			close(ep);
			close(fds[0]);

			if (argc == 1){
				work(0, fds[1]);
			} else {
				int fd = open(argv[i+1], O_RDONLY);
				if (fd == -1){
					perror("open");
					exit(0);

				}
				work(fd, fds[1]);
				close(fd);

			}
			close(fds[1]);
			exit(0);
		} 



		}
	}



	return 0;
}
