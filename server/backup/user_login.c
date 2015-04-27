#include "server.h"
#include "link.h"

int user_login(const char *data,char *username)
{
	FILE *file;
	char buf[100];
	char *p = NULL;
	int flag = -1;

	p = strchr(data,'=');
	strncpy(username,data,p-data);
	username[p-data] = '\0';

	pthread_mutex_lock(&name_list_mutex);
	if(is_in_link(name_list,username))
		return -1;
	pthread_mutex_unlock(&name_list_mutex);

	pthread_mutex_lock(&database_mutex);
	file = fopen(DataBasePath,"r");
	if(file == NULL)
		perror("failed to open DataBasePath");

	while(fgets(buf,99,file) != NULL){
		buf[strlen(buf)-1] = '\0';
		if(strcmp(buf,data) == 0){
			flag = 0;
			break;
		}
	}
	fclose(file);
	pthread_mutex_unlock(&database_mutex);

	return flag;
}

/*int main(int argc, char **argv)
{
	char username[50];
	int i;

	i = user_login("zhbo=123",username);
	printf("i:%d\tusername:%s\n",i,username);
}*/
