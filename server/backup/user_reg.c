#include "server.h"

#define MAXLEN	1024
int parse_data(char *data, char u_name[], char password[])
{
	char *p;

	p=strchr(data, '=');
	if(p == NULL)
		return -1;
	strncpy(u_name, data, p-data);
	u_name[p-data]='\0';
	strcpy(password, p+1);

	return 0;
}
int user_reg(int connfd, char *data)
{
	FILE *f;
	char *p;
	char buf[MAXLEN];
	char u_name[50], name[50];
	char password[50];

	parse_data(data, u_name, password);

	pthread_mutex_lock(&database_mutex);	
	f = fopen(DataBasePath, "r+");
	if(f==NULL){
		perror("fail to open");
		return 1;
	}

	while(fgets(buf, MAXLEN,f)!=NULL){
		p=strchr(buf, '=');
		if(p == NULL)
			return -1;
		*p = '\0';
		strcpy(name, buf);
		if(strcmp(u_name, name)==0){
			my_write(connfd, "reg:#:#:rename", strlen("reg:#:#:rename"));
			return -1;
		}
	}
	strcat(data,"\n");
	fseek(f,0,SEEK_END);
	fwrite(data,sizeof(char),strlen(data),f);
	//	fputs(data,f);
	fclose(f);
	sync();

	pthread_mutex_unlock(&database_mutex);	
	my_write(connfd, "reg:#:#:ok", strlen("reg:#:#:ok"));
	close(connfd);
	return 0;
}
/*
   int main()
   {
   user_reg(5, "aaa=bbb");

   return 0;
   }*/
