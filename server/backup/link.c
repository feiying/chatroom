#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "link.h"

user_link NODE(pthread_t tid,int connfd, char user_name[], int status, user_link next)
{
	user_link t=(user_link)malloc(sizeof *t);
	t->tid=tid;
	t->connfd=connfd;
	strcpy(t->user_name, user_name);
	t->status=status;
	t->next=next;
	return t;
}

void show_list(user_link t)
{
	t=t->next;
	while(t)
	{
		printf("%d\n",t->connfd);
		printf("%s\n\n",t->user_name);
		t=t->next;

	}
	printf("\n");
}

user_link init_list()
{
	user_link t=NODE(0,0,"",0,NULL);
	user_link head=t;
	return head;
}
int delete_node(user_link t,int connfd)
{
	user_link x=NULL,y=NULL;
	x = t;
	y=t->next;
	while(y!=NULL)
	{
		if(connfd==y->connfd) break;
		x=y;
		y=y->next;
	}
	if(y==NULL)
	{
		printf("can't find\n");
		return -1;
	}
	x->next=y->next;
	free(y);
	return 0;
}

user_link insert_node(user_link t,pthread_t tid, int connfd, char user_name[], int status)
{
	user_link x,y=t;
	x=y;
	while(y!=NULL)
	{
		x=y;
		y=y->next;
	}
	x=x->next=NODE(tid, connfd, user_name, status, y);
	return t;
}

int is_in_link(user_link t, char *user_name)
{
	user_link x = NULL;
	
	for(x = t; x != NULL; x = x->next)
		if(!strcmp(user_name,x->user_name))
			return 1;

	return 0;
}

user_link distory_list(const user_link t)
{
	user_link x=t;
	user_link tmp=x->next;
	while(x)
	{
		x=tmp->next;
		free(tmp);
		tmp=NULL;
		tmp=x;
	}
	t->next=NULL;
	return t;
}

