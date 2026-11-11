#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define P 5  //повара
#define W 10 //места
#define S 3  //склад
#define Q 2  //курьеры
#define N 15

typedef struct {
	char kitchen[W][6];
	char storage[S][6];
	int count; //число пицц на складе сейчас
	int cooked;


	pthread_mutex_t m;

	pthread_cond_t c_chef;
	pthread_cond_t c_del;
} Monitor;


void* chef(void* param){ //повар
	Monitor* mn = (Monitor*)param;
	while(1){
		pthread_mutex_lock(&mn->m);

		int place = -1; //ищем пиццу с которой можно что-то сделать
		while (1){
			if (mn->cooked >= N){
				pthread_mutex_unlock(&mn->m);
				return NULL;
			}
			

			for (int i=0; i<W; i++){
				if (strcmp(mn->kitchen[i], "pizza")||mn->count<S){
					place = i;
					break;
				}

			}
			if (place != -1) break;
			pthread_cond_wait(&mn->c_chef, &mn->m);
		}
		if (strcmp(mn->kitchen[place], "xxxxx") == 0){
			strcpy(mn->kitchen[place], "pxxxx");
			printf("повар добавил тесто на пиццу номер %d\n", place);
		}
		else if (strcmp(mn->kitchen[place], "pxxxx") == 0){
			strcpy(mn->kitchen[place], "pixxx");
			printf("повар добавил начинку на пиццу номер %d\n", place);
		}
		else if (strcmp(mn->kitchen[place], "pixxx") == 0){
			strcpy(mn->kitchen[place], "pizxx");
			printf("повар добавил соус1 на пиццу номер %d\n", place);
		}
		else if (strcmp(mn->kitchen[place], "pizxx") == 0){
			strcpy(mn->kitchen[place], "pizzx");
			printf("повар добавил соус2 на пиццу номер %d\n", place);
		}
		else if (strcmp(mn->kitchen[place], "pizzx") == 0){
			strcpy(mn->kitchen[place], "pizza");
			printf("повар испек пиццу номер %d\n", place);
		}
		else if (strcmp(mn->kitchen[place], "pizza") == 0){
			strcpy(mn->storage[mn->count], mn->kitchen[place]);
			mn->count += 1;
			mn->cooked +=1;
			strcpy(mn->kitchen[place], "xxxxx");
			printf("повар отнес пиццу номер %d\n", place);

			pthread_cond_signal(&mn->c_del);
		}
	
		pthread_mutex_unlock(&mn->m);	
	
	}	
	return NULL;
}

void* deliver(void* param){ //курьер
	Monitor* mn = (Monitor*)param;
	while (1){
		pthread_mutex_lock(&mn->m);
		while (mn->count == 0) { //если склад пока пустой - ждем
			pthread_cond_wait (&mn->c_del, &mn->m);
		}
		mn->count -= 1;

		char box[6];
		strcpy(box, mn->storage[mn->count]); //забираем последнюю пиццу со склада

		if (strcmp(box, "pizza") == 0){
			strcpy(box, "xxxxx");
			printf("Пицца правильная\n");
		}
		pthread_cond_signal(&mn->c_chef);
		pthread_mutex_unlock(&mn->m);

	}
	return NULL;
}


int main(){
	Monitor mn;
	for (int i=0; i<W; i++){
		strcpy(mn.kitchen[i], "xxxxx");
	}
	mn.count = 0;
	pthread_mutex_init(&mn.m, NULL);
	pthread_cond_init(&mn.c_chef, NULL);
	pthread_cond_init(&mn.c_del, NULL); //начальное состояние монитора

	

	pthread_t chefs[P];
	for (int i=0; i<P; i++){
		int s = pthread_create(&chefs[i], NULL, chef, &mn);
	}

	pthread_t delivers[Q];
	for (int i=0; i<Q; i++){
		int s = pthread_create(&delivers[i], NULL, deliver, &mn);
	}



	for (int i=0; i<P; i++){
		int s = pthread_join(chefs[i], NULL);

	}
	for (int i=0; i<Q; i++){
		int s = pthread_join(delivers[i], NULL);
	}


	return 0;
}
