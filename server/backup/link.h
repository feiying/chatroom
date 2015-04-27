#include "server.h"
#ifndef __LINK_H__
#define __LINK_H__
typedef struct node *user_link;
struct node 
{
	pthread_t tid;
	int connfd;
	char user_name[30];
	int status; 
	user_link next;
};

user_link NODE(pthread_t tid,int connfd, char user_name[], int status, user_link next);
user_link init_list();
void show_list(user_link t);
int delete_node(user_link t,int connfd);
int is_in_link(user_link t, char *user_name);
user_link insert_node(user_link t,pthread_t tid, int connfd, char user_name[], int status);
user_link distory_list(const user_link t);

extern user_link name_list;
#endif
