#include "link.h"
#include "server.h"
#define MAXLINE 2048

int add_user_list(int connfd, char *username, int status)
{
	pthread_t tid;
	tid = pthread_self();
	pthread_mutex_lock(&name_list_mutex);
	insert_node(name_list,tid,connfd,username,status);
	pthread_mutex_unlock(&name_list_mutex);
	
	return 0;
}
int refresh()
{
	char buf[MAXLINE],name[50];
	int arr[50];
	int count = 0;
	user_link x = NULL;
	int i;

	pthread_mutex_lock(&name_list_mutex);
	for(x = name_list->next;x != NULL; x = x->next)
	{
		strcpy(name,x->user_name);
		strcat(name,"\n");
		strcat(buf,name);
		arr[count] = x->connfd;
		count++;
	}
	for(i = 0; i < count; i++)
		my_write(arr[i],buf,strlen(buf));
	pthread_mutex_unlock(&name_list_mutex);

	return 0;
}
int del_user_list(int connfd)
{
	int i;

	pthread_mutex_lock(&name_list_mutex);
	i = delete_node(name_list,connfd);
	pthread_mutex_unlock(&name_list_mutex);

	return i;
}
