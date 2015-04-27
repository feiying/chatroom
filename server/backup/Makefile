all: chat chatg client

chat : 
	gcc -Wall maint.c serinit.c link.c user_reg.c user_login.c list_handler.c  parse_command.c chat.c -o chat -lpthread

chatg: 
	gcc -Wall maint.c serinit.c link.c user_reg.c user_login.c list_handler.c  parse_command.c chat.c -o chatg -lpthread -g

client:
	gcc -Wall client.c parse_command.c -o client -lpthread
