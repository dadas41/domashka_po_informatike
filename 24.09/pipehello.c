#include <stdio.h>
#include <unistd.h>


int main(){
	int fds[2];

	if (pipe(fds)<0){
		perror("pipe");
		return 0;
	}
	
	int p = fork();
	if (p == 0){
		write(fds[1], "hello world", 12);
		return 0;
	}
	
	char buf[12];

	read(fds[0], buf, 12);
	write(1, buf, 12);


	close(fds[0]);
	close(fds[1]);

	return 0;
}
