#include "server.h"
#include "link.h"
#include <time.h>
#define MAXLEN 1024
struct tm *myctime()
{
	time_t t=time(NULL);
	struct tm *tm1=localtime(&t);
	tm1->tm_year+=1900;
	tm1->tm_mon+=1;

	return tm1;

}
int write_chat_record(struct command *cmdline,char *ip)
{
	struct tm *tml;
	tml=myctime();
	char buf[1024];

	pthread_mutex_lock(&chatrecord_mutex);
	sprintf(buf, 
			"[%d-%d-%d %d:%d:%d][%s][%s][%s]:\n%s\n",
			tml->tm_year,tml->tm_mon,tml->tm_mday,tml->tm_hour,tml->tm_min,tml->tm_sec,
			ip,
			cmdline->source,
			cmdline->dest,
			cmdline->data);
	fwrite(buf,sizeof(char),strlen(buf),fp);
	sync();
	pthread_mutex_unlock(&chatrecord_mutex);
	return 0;
}

int chat(struct user_connect *p)
{
	int n;
	char buf[MAXLEN];
	char buf_answer[50];
	struct command cmdline;
	user_link u_link;
	while(1){
		n=my_read(p->connfd, buf, MAXLEN);
		if(n <= 0){		/** user quit abnomaly **/
			del_user_list(p->connfd);
			refresh();
			pthread_exit((void *)1);
		}

		parse_command(&cmdline, buf);

		if(strcmp(cmdline.cmd, "chat")==0){
			if(strcmp(cmdline.dest, "all")==0){	/** to all **/
				pthread_mutex_lock(&name_list_mutex);
				for(u_link=name_list->next; u_link!=NULL; u_link=u_link->next)
					if(u_link->connfd != p->connfd){
						my_write(u_link->connfd, buf, strlen(buf));
					}

				/** tell the source that the message has sent to all, you can send the next message **/
/*				strcat(buf_answer,"answer:");
				strcat(buf_answer,cmdline.dest);
				strcat(buf_answer,":");
				strcat(buf_answer,cmdline.source);
				strcat(buf_answer,":ok");
				buf_answer[strlen(buf_answer)]='\0';
				my_write(p->connfd, buf_answer, strlen(buf_answer));
*/
				/** write the chat record **/
				write_chat_record(&cmdline,p->ip);
				pthread_mutex_unlock(&name_list_mutex);
			}
			else{	/** to target **/
				pthread_mutex_lock(&name_list_mutex);
				for(u_link=name_list->next; u_link!=NULL; u_link=u_link->next){
					if(strcmp(u_link->user_name, cmdline.dest)==0)
						break;
				}
				pthread_mutex_unlock(&name_list_mutex);
				if(u_link != NULL){
					my_write(u_link->connfd, buf, strlen(buf));
					/** write the chat record **/
					write_chat_record(&cmdline,p->ip);
				}
				else{
					my_write(p->connfd,"the man is not online\n",strlen("the man is not online\n"));
					printf("the man is not online\n");
				}
			}
		}
		else if(strcmp(cmdline.cmd, "quit")==0){	/*** user quit normaly ***/
			del_user_list(p->connfd);
			refresh();
			pthread_exit((void *)1);
		}
		else if(strcmp(cmdline.cmd, "answer")==0){	/*** target recive the message ***/
			if(strcmp(cmdline.data, "ok")==0){
				my_write(p->connfd, "receive:#:#:ok", strlen("receive:#:#:ok"));
				pthread_mutex_lock(&name_list_mutex);
				for(u_link=name_list->next; u_link!=NULL; u_link=u_link->next){
					if(strcmp(u_link->user_name, cmdline.dest)==0)
						break;
				}
				pthread_mutex_unlock(&name_list_mutex);
				if(u_link != NULL){
					my_write(u_link->connfd, buf, strlen(buf));
				}
			}
		}
		else{
			my_write(p->connfd,"the cmd is not chat",strlen("the cmd is not chat"));
			printf("the command is not chat\n");
		}
		/** clear the buffer **/
		bzero(buf, MAXLEN);

	}

	return 0;
}

/*int main()
  {
  return 0;
  }*/
