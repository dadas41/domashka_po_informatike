#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


int main(){
	int flag_exit = 0;
	char line[512];
	while (!flag_exit){
		write(1, "my$ ", 4);

		int r = read(0, line, 512);
		line[r-1] = '\0'; //надо убрать \n

		if (strcmp(line, "myexit") == 0) flag_exit = 1;

		if (strlen(line) == 0) continue;


		int count = 0;
		char* args[20];
		int arg_count = 0;

		int fd_number = 0;  //чтобы команды лесенкой
		int fds[2];
		

		char* words = strtok(line, " ");
		while (words != NULL){
			if (strcmp(words, "|") == 0){
				
				if (pipe(fds) < 0){
					perror("pipe");
					break;
				}

			

				int p = fork();
				if (p == 0){	
					close(fds[0]);
					dup2(fds[1], 1);
					close(fds[1]);
				
					execvp(args[0], args);
				}
				count++;
				if (fd_number != 0) close(fd_number);
				close(fds[1]);
				fd_number = fds[0];

				arg_count = 0;
			}
			else {
				if (arg_count < 19){
					args[arg_count++] = words;
				}
			}
			words = strtok(NULL, " ");
			
		}
		if (arg_count > 0){
			args[arg_count] = NULL;
			int p = fork();
			if (p == 0){
				if (fd_number != 0){
					dup2(fd_number, 0);
					close(fd_number);

				}
				execvp(args[0], args);
			}
			count++;

	
		}
		if (fd_number != 0) close(fd_number);


		for (int i=0; i<count; i++){
			wait(0);
		}
		
	}
	return 0;
	

	
}
