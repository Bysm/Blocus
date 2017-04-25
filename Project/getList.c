#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>

int main ( void ){
	char *temp[] = {NULL,"test",NULL};
	temp[0]="get-easy.sh";
	execve("get-easy.sh",temp,NULL);
	printf("Un probleme est survenu pour la mise a jour de la EasyList. \nVérifiez que vous avec toutes les dépendances nécessaire");
	return 1;
}