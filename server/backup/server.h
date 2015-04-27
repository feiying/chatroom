#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<pthread.h>

#ifndef _SERVER_
#define _SERVER_
struct user_connect{
	char ip[16];
	int connfd;
};
struct command{
	char cmd[8];
	char source[30];
	char dest[30];
	char data[160];
};

int getconf(char *,short *,char *);
void ser_bind(int , char *);
int user_login(const char *, char *);
int user_reg(int, char *);
int parse_command(struct command *, char *);
char *IntToString(char *, int);
int my_read(int connfd, char *buf, int buf_size);
int my_write(int connfd, char *data, int n);
int add_user_list(int, char *, int);
int refresh();
int del_user_list(int);
int chat(struct user_connect *);

extern pthread_mutex_t name_list_mutex;
extern pthread_mutex_t chatrecord_mutex;
extern pthread_mutex_t database_mutex;
extern char DataBasePath[256];
extern FILE *fp;
#endif

