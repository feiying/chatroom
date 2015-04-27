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
	char source[50];
	char dest[50];
	char *data;
};
int getconf(char *,short *,char *);
void ser_bind(int , char *);

extern pthread_mutex_t name_list_mutex;
extern pthread_mutex_t chatrecord_mutex;
extern pthread_mutex_t database_mutex;
extern char DataBasePath[256];
extern FILE *fp;
#endif

