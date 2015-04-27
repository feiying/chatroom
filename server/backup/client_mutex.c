#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netinet/in.h>
#include<pthread.h>
#include "server.h"
pthread_mutex_t s_mutex=PTHREAD_MUTEX_INITIALIZER;
static char mutex_flag=0;

void *thr_read(void *arg)
{
	int sockfd = (int)arg;
	int len;
	char recvline[1024];
	while(1){
		if(mutex_flag == 0){
			len = read(sockfd,recvline,1024);
			if(len == 0){
				printf("the other side has been closed.\n");
				return((void *)1); 
			}

			recvline[len] = '\0';
			printf("%s\n",recvline);
			mutex_flag = 1;
		}
	}

}

void *thr_write(void *arg)
{
	int sockfd = (int)arg;
	char buf[1024];
	int len;
	while(1){
		if(mutex_flag == 1){
			fgets(buf,1024,stdin);
			len = strlen(buf);
			buf[len-1] = '\0';

			write(sockfd,buf,len);
			mutex_flag = 0;
		}
	}
}

int main(int argc, char * argv[])
{
	int sockfd, n, counter = 0;
	struct sockaddr_in servaddr;
	int len;
	pthread_t t_read_id, t_write_id;

	if(argc != 3){
		printf("usage:a.out<ipaddress><ports>\n");
		exit(0);
	}
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("socket error\n");
		exit(0);
	}
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr) <= 0){
		printf("inet_pton error for %s\n",argv[1]);
		exit(0);
	}
	if(connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0){
		printf("connect error\n");
		exit(0);
	}

	printf("connect ok\n");

	pthread_create(&t_read_id, NULL, thr_read,(void *)sockfd);
	pthread_create(&t_write_id, NULL, thr_write,(void *)sockfd);

	pthread_join(t_read_id,NULL);
	pthread_join(t_write_id,NULL);
	while(1)
	{}
	close(sockfd);
	//printf("ok\n");
	//sleep(30);
	return 0;
}
