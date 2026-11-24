#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/select.h>



//эта версия с select, версия с o_nonblocked лежит в папке 19.11




void count_func(char* buf, int n, int* byte, int* word, int* line){
	*byte += n;
		
	for (int i=0; i<n; i++){
		char c = buf[i];
		if (c == '\n') {
			*line += 1;
			if (i!= 0 && buf[i-1] != ' ') *word += 1;

		}
		else if (c == ' ' && i != 0 && buf[i-1] != ' '){
			*word += 1;
			
		}
		else if (i==n-1 && c != ' ') *word += 1;
	}

}	


int counter(int fds_out[2], int fds_err[2]){	
	close(fds_out[1]);
	close(fds_err[1]);
	
	int byte1 = 0, byte2 = 0;
	int word1 = 0, word2 = 0;
	int line1 = 0, line2 = 0;

	char buf[4096];
	int n = 0;
	int flag_out = 1, flag_err = 1;
	
	int nfds = 0;

	if (fds_out[0] > fds_err[0]){
		nfds = fds_out[0] + 1; 
	} else {
		nfds = fds_err[0] + 1;
	}


	while (flag_out || flag_err){
		fd_set readfds;
		FD_ZERO(&readfds);
		
		FD_SET(fds_out[0], &readfds);
		FD_SET(fds_err[0], &readfds);

		int ready = select(nfds, &readfds, NULL, NULL, NULL);
		if (ready == -1){
			return 0;
		}


		if (flag_out && FD_ISSET(fds_out[0], &readfds)){
			n = read(fds_out[0], buf, sizeof(buf));
			if (n > 0){
				count_func(buf, n, &byte1, &word1, &line1);			
			} else flag_out = 0;
		}
		if (flag_err && FD_ISSET(fds_err[0], &readfds)){
			n = read(fds_err[0], buf, sizeof(buf));
			if (n > 0){
				count_func(buf, n, &byte2, &word2, &line2);
			} else flag_err = 0;
		}

	}
	close(fds_out[0]);
	close(fds_err[0]);

	printf("stdout:\n");
	printf("Byte: %d\n", byte1);
	printf("Word: %d\n", word1);
	printf("Line: %d\n", line1);
	
	printf("stderr:\n");
	printf("Byte: %d\n", byte2);
	printf("Word: %d\n", word2);
	printf("Line: %d\n", line2);
	
	return 0;
}

int main(int argc, char* argv[]){
	struct rusage usage;
	int status;

	int fds_out[2];
	if (pipe(fds_out)<0){
		perror("pipe");
		return 0;
	}

	int fds_err[2];
	if (pipe(fds_err)<0){
		perror("pipe");
		return 0;
	}
	int p = fork();
	if (p == 0){
		close(fds_out[0]);
		close(fds_err[0]);

		dup2(fds_out[1], 1);
		dup2(fds_err[1], 2); 
		
		close(fds_out[1]);
		close(fds_err[1]);


		execvp(argv[1], &argv[1]); 
		return 0;
	}
	
	

	counter(fds_out, fds_err);

	wait(0);

	return 0;	
}
