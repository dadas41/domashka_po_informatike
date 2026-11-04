#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <getopt.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

int flag_1 = 0;
int flag_l = 0;
int flag_a = 0;
int flag_i = 0;
int flag_n = 0;
int flag_r = 0;

void print_long(const char* name, struct stat* st){
	if (flag_i) {
		printf("%lu ", (unsigned long)st->st_ino);
	}
	
	
	if (S_ISDIR(st->st_mode)) printf("d");
	else if (S_ISLNK(st->st_mode)) printf("l");
	else printf("-");

	for (int i = 0; i<3; i++){
		int m = (st->st_mode >> ((2-i)*3)); //6 для 0, 3 для 1, 0 для 2
		printf((m&4) ? "r" : "-");
		printf((m&2) ? "w" : "-");
		printf((m&1) ? "x" : "-");
	}
	printf(" %lu", (unsigned long)st->st_nlink);
	
	if (flag_n){
		printf(" %d", st->st_uid);
	}else{
		struct passwd* pw = getpwuid(st->st_uid);
		printf(" %s", pw->pw_name);
	}

	if (flag_n){
		printf(" %d", st->st_gid);
	} else{
		struct group* gr = getgrgid(st->st_gid);
		printf(" %s", gr->gr_name);
	}


	printf(" %ld", st->st_size);

	char time_buf[64];
	struct tm* time = localtime(&st->st_mtime);
	strftime(time_buf, sizeof(time_buf), "%b %e %R", time);
	printf(" %s %s\n", time_buf, name);

}


void scan_dir(char* path){
	DIR* d = opendir(path);
	struct dirent* e;
	while ((e = readdir(d)) != 0){
		if (!flag_a && e->d_name[0] == '.') continue;
		if (flag_l || flag_n){
			char full_path[1024];
			snprintf(full_path, sizeof(full_path), "%s/%s", path, e->d_name);
			
			struct stat st;
			if (stat(full_path, &st) == 0){
				print_long(e->d_name, &st);
			}
		
		} else if (flag_1){
			if (flag_i){
				printf("%lu ", (unsigned long)e->d_ino);
			}
			printf("%s\n", e->d_name);

		} else{
			if (flag_i){
				printf("%lu ", (unsigned long)e->d_ino);
			}
			printf("%s ", e->d_name);
		}
	}
	if (!flag_l && !flag_1){
		printf("\n");
	
	}	
	closedir(d);
}

int main(int argc, char* argv[]){
	struct option options[] = {
		{0, 0, 0, 0},
		{"long", no_argument, 0, 'l'},
		{"all", no_argument, 0, 'a'},
		{"inode", no_argument, 0, 'i'},
		{"num", no_argument, 0, 'n'}
	};
	int opt;
	while ((opt = getopt_long(argc, argv, "l1ain", options, NULL)) != -1){
		switch (opt) {
			case 'l':
				flag_l = 1;
				break;
			case '1':
				flag_1 = 1;
				break;
			case 'a':
				flag_a = 1;
				break;
			case 'i':
				flag_i = 1;
				break;
			case 'n':
				flag_n = 1;
				break;
			default:
				return 1;
		}
	}
	if (optind == argc){
		scan_dir(".");
		return 0;
	} 


	int flag_file = 0;
	for (int i = optind; i<argc; i++){
		struct stat st;
		if (stat(argv[i], &st) != 0){
			perror(argv[i]);
			continue;
		}
		if (S_ISDIR(st.st_mode)) {
			if (argc - optind > 1){
				printf("%s:\n", argv[i]);
			}
			scan_dir(argv[i]);
			if (i < argc - 1) printf("\n");

		} else {
			if (flag_l || flag_n){
				print_long(argv[i], &st);
				
			} else if (flag_1){
				printf("%s\n", argv[i]);
			
			}else {
				printf("%s ", argv[i]);
				flag_file = 1;
			}

		}

	}
	if (flag_file && !flag_1 && !flag_l && !flag_n) printf("\n");

	return 0;	
	

}
