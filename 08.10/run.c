/*
 * reg = 1
 * stick = 2
 * n = runners
judge()	
	print start registr
	for (n)
		msgrcv(reg)
		print runner i reg
	print start run
	msgsnd(stick)
	waiting for finish
	msgrcv(n+stick)

	print time


runner() i = номер бегуна - 1     [0, n-1]
	print я создался

	msgsnd reg

	msgrcv stick + i
	print получил stick + i

	next = i+1

	msgsnd stick + i + 1
	print отдал палку

main()
	create msg
	p = fork
	if p = 0
		judge()
	for (n)
		x = fork()
		if x = 0
			runner()
	wait
	msgctl

*/



#include <stdio.h>
#include <sys/msg.h>

struct msg_buf{
	long mtype;
	int mid;
};

void judge(int n, int id){
	struct msg_buf buf;
	printf("Регистрация началась\n");

	for (int i=0; i<n; i++){
		msgrcv(id, &buf, sizeof(buf.mid), 999, 0);
	     	printf("Бегуна %s зарегестрировали\n", i+1);  

	}
	printf("Начало забега\n");
	buf.mtipe = 1;
	buf.mid = 0;

	printf("Передаю палочку 1 бегуну\n");
	msgsnd(id, &buf, sizeof(buf.mid), 0);
	
	msgrcv(id, &buf, sizeof(buf.mid), n+1, 0);
	
	printf("Последний бегун прибежал, общее время\n");
	printf("Конец забега\n");
}

void runner(int i, int id){
	struct msg_buf buf;
	buf.mtype = 999;
	buf.mid = i;

	printf("Начинаю регистрацию, %d \n", i);
	msgsnd(id, &buf, sizeof(buf.mid), 0);

	msgrcv(id, &buf, sizeof(buf.mid), i, 0);
	printf("Получил %d, начал бежать\n", i);
	
	buf.mtype = i + 1;
	msgsnd(id, &buf, sizeof(msg.mid), 0);

}


int main(){
	int id = msgget(IPC_PRIVATE, IPC_CREAT | 0700);
	int p = fork();
	if (p == 0){
		judge(id);
		return 0;
	}	
	for (int i = 1; i<n+1; i++){
		int x = fork();
		if (x == 0) {
			runner(i, id);
			return 0:
		}
	}
	for (int i = 0; i<n+1; i++) wait(0);

	msgctl(id, IPC_RMID, 0);

}
