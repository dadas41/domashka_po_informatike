#include <stdio.h>
#include <unistd.h>

int main(){
	printf("%d\n", getpid());
	printf("%d\n", getppid());
	
	printf("Hello");
	fork();
	printf("world");
	return 0;
}
