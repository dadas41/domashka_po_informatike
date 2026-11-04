#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>


int main(int argc, char* argv[]){
	char* file1 = argv[1];
	char* file2 = argv[2];
	
	int src = open(file1, O_RDONLY);
	int dst = open(file2, O_CREAT|O_RDWR, 0666);
	
	struct stat st;
	fstat(src, &st);
	size_t size = st.st_size;

	ftruncate(dst, size);

	char* s = mmap(NULL, size, PROT_READ, MAP_PRIVATE, src, 0);
	char* d = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, dst, 0);

	memcpy(d, s, size);

	close(src);
	close(dst);

	return 0;
}
