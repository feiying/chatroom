#include "server.h"
#include "link.h"

void ser_bind(int listenfd,char *ChatRecordPath)
{
	struct sockaddr_in servaddr;
	int opt = 1;
	short port;

	if(getconf("./config/server.conf",&port,ChatRecordPath)<0)
		printf("getconf error\n");

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));	
	name_list = init_list();
	printf("port:%d DataPath:%s Chatrecord:%s\n",port,DataBasePath,ChatRecordPath);
}

int getconf(char *str,short *port,char *ChatRecordPath)
{
	FILE *file;
	char buffer[1024];
	char *p;
	
	file = fopen(str,"r");
	if(file == NULL){
		perror("open file");
		return -1;
	}

	while(fgets(buffer,1024,file)){
		buffer[strlen(buffer)-1]='\0';
		p = strchr(buffer,'=');
		if(p == NULL)
			return -1;
		*p = '\0';
		if(!strcmp(buffer,"Port"))
			*port = (short)atoi(p+1);
		else if(!strcmp(buffer,"DataBasePath"))
			strcpy(DataBasePath,p+1);
		else if(!strcmp(buffer,"ChatRecordPath"))
			strcpy(ChatRecordPath,p+1);
		else
		{
		}
	}

	return 0;
}

