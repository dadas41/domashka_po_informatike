#include <pthread.h>
#include <stdio.h>

void* func(void* param){
	int* p = (int*) param;
	for (int i=0; i<1000000; i++){
		(*p)++;
	}
	return NULL;
}


int main(){
	int count = 0;
	pthread_t threads[10];
	for (int i=0; i<10; i++){
		int s = pthread_create(&threads[i], NULL, func, &count);
		
	}
	for (int i=0; i<10; i++){
		int s = pthread_join(threads[i], NULL); 
	}

	printf("%d\n", count);
	return 0;
}
