//у меня ничего не получилось(
//сначала пыталась через ненадежные сигналы, 
//но никуда ничего не пишется
//
//пыталась начать через ненадежные
//до конца не разобралась



#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

//переменные для хендлера1
int byte = 0;
int bit = 0;
pid_t child = -1;

//переменные для хендлера2
int fd = -1;
int bit2 = 0;


sigset_t mask, oldmask;
int term = 0;


void handler1(int sig){
	if (sig == SIGTERM){
		if (bit != 0){
			char c = (char)byte;
			write(STDOUT_FILENO, &c, 1);
		}
		exit(0);
	}
	if (sig == SIGUSR2){
		byte |= (1 << bit);
	}
	bit++;

	if (bit == 8){
		char c = (char)byte;
		write(STDOUT_FILENO, &c, 1);
		byte = 0;
		bit = 0;
	}
}

void handler2(int sig){
	char b;
	if (read(fd, &b, 1) != 1){
		kill(getppid(), SIGTERM);
		_exit(0);
	}
	int bit0 = (b >> bit2) & 1;
	bit2 = (bit2 + 1) % 8;

	if (bit0 == 0) kill(getppid(), SIGUSR1);
	else kill(getppid(),SIGUSR2);
}


int main(int argc, char* argv[]){
	sigemptyset(&mask);
	sigaddset(&mask, SIGUSR1);
	sigaddset(&mask, SIGUSR2);
	sigaddset(&mask, SIGTERM);
	sigprocmask(SIG_BLOCK, &mask, &oldmask);

	signal(SIGUSR1, handler1);
	signal(SIGUSR2, handler1);
	signal(SIGTERM, handler1);
	
	child = fork();
	if (child == 0){
		signal(SIGUSR1, handler2);
		signal(SIGUSR2, SIG_IGN);
		signal(SIGTERM, SIG_DFL);
		fd = 0;
		if (argc > 1) {
			fd = open(argv[1], O_RDONLY);
		}	
		if (fd <0) {
			perror("open");
			_exit(1);
		}
		
		sigprocmask(SIG_SETMASK, &oldmask, NULL);

		handler2(0);
		while(1){
			pause();
		}
	}

	
	while (1){
		sigsuspend(&oldmask);
		kill(child, SIGUSR1);
	
	}

	return 0;
}
