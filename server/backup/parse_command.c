#include "server.h"

int parse_command(struct command *cmdline,char *buf)
{
	char *p = NULL;
	char *q = buf;
	int i = 0;

	/** clear the cmdline **/
	bzero(cmdline->cmd, sizeof(cmdline->cmd));
	bzero(cmdline->source, sizeof(cmdline->source));
	bzero(cmdline->dest, sizeof(cmdline->dest));
	bzero(cmdline->data, sizeof(cmdline->data));


	while((p = strchr(q,':')) != NULL)
	{
		if(i >= 3)
			break;

		//	*p = '\0';
		if(i == 0){
			strncpy(cmdline->cmd, q, p-q);
			(cmdline->cmd)[p-q]='\0';
			q = p+1;
		}
		else if(i == 1){
			strncpy(cmdline->source,q, p-q);
			(cmdline->source)[p-q]='\0';
			q = p+1;
		}
		else if(i == 2){
			strncpy(cmdline->dest,q, p-q);
			(cmdline->dest)[p-q]='\0';
			q = p+1;
		}

		i++;
		//	buf = p+1;
	}
	strcpy(cmdline->data,q);

	return 0;
}

char *IntToString(char a[], int i)
{
	char tmp[5];
	char *p = tmp, *q;
	while(1){
		*p = (i%10)+'0';
		p++;
		i = i/10;
		if(i == 0){
			*p = '\0';
			break;
		}
	}
	p--;
	for(q = a; p!=tmp-1; q++,p--){
		*q = *p;
	}
	*q='\0';
	return a;

}

int my_read(int connfd, char *buf, int buf_size)
{
	int len, n;
	n = read(connfd, buf, 4);
	if(n <= 0)
		return -1;
	buf[n]='\0';
	len = atoi(buf);
	if(len == 0){
		printf("bad command!\n");
		return -1;
	}
	bzero(buf, sizeof(buf));
	n = read(connfd, buf, len);
	if(n <= 0)
		return -1;
	buf[n]='\0';
	
	return n;
}

int my_write(int connfd, char *data, int n)
{
	char len[5];
	write(connfd, IntToString(len, n), 4);
	write(connfd, data, n);
	return 0;
}
/*int main(int argc, char **argv)
  {
  char buf[256];
  struct command cmdline;

  strcpy(buf,"login:#:#:zhbo=123");
  parse_command(&cmdline,buf);
  printf("cmd:%s\nsource:%s\ndest:%s\ndata:%s\n",cmdline.cmd,cmdline.source,cmdline.dest,cmdline.data);
  }*/
