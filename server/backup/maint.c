#include "server.h"
#include "link.h"

#define MAXLINE 2048

char DataBasePath[256];
FILE *fp;
user_link name_list;
pthread_mutex_t name_list_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t chatrecord_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t database_mutex = PTHREAD_MUTEX_INITIALIZER;

void *tfn(void* arg)
{
	char buf[MAXLINE];
	struct user_connect p;
	struct user_connect *tmp;
	struct command cmdline; 
	char username[50];
	int status = 0;
	int n;

	tmp = (struct user_connect *)arg;
	strcpy(p.ip,tmp->ip);
	p.connfd = tmp->connfd;
	
	n = my_read(p.connfd, buf, MAXLINE);
	if(n <= 0)
		return NULL;
	buf[n]='\0';
	parse_command(&cmdline, buf);
	if(strcmp(cmdline.cmd,"reg")== 0){
		user_reg(p.connfd,cmdline.data);	/** user regedit **/
		pthread_exit((void*)1);
	}
	else if(strcmp(cmdline.cmd,"login") == 0){
		if(user_login(cmdline.data,username) == 0){
			add_user_list(p.connfd,username,status);
			refresh();
			chat(&p);
		}else{
			my_write(p.connfd,"login:#:#:error",strlen("login:#:#:error"));
			close(p.connfd);
			pthread_exit((void *)1);
		}
	}
	else{
		my_write(p.connfd,"bad command\n",strlen("bad command\n"));
	}

	return NULL;
}
int main(void)
{
	struct sockaddr_in cliaddr;
	struct user_connect u_con;
	socklen_t cliaddr_len;
	int  connfd,listenfd;
	char str[INET_ADDRSTRLEN];
	char ChatRecordPath[256];
	int err;
	pthread_t tid;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	ser_bind(listenfd,ChatRecordPath);
	listen(listenfd, 30);
	signal(SIGPIPE,SIG_IGN);

	fp = fopen(ChatRecordPath,"a");
	if(fp == NULL)
		perror("falied to open");
	printf("Accepting connections...\n");
	while(1){
		cliaddr_len = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
		//printf("ip :%s\n",inet_ntop(AF_INET, &cliaddr.sin_addr,str,sizeof(str)));
		strcpy(u_con.ip,inet_ntop(AF_INET, &cliaddr.sin_addr,str,sizeof(str)));
		u_con.connfd = connfd;
		err = pthread_create(&tid,NULL,tfn,(void *)&u_con);
		if(err != 0){
			fprintf(stderr,"can't create thread:%s\n",strerror(err));
			return -1;
		}
	}
	return 0;
}
