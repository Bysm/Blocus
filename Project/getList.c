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
	
	char *temp[] = {NULL,"silence",NULL};
	temp[0]="get-easy.sh";
	execve("get-easy.sh",temp,NULL);
	printf("la fin \n");
	
	/*FILE * list= NULL;
	list = fopen("list.txt","r+");
	char *ret= NULL;
	int num = 1;
	char temp[200];
	char tempp[20] = "popunder";
	if(list != NULL){
		// si ret == NULL alors il ne contient pas le mot dans la chaine
		while((fgets(temp,200,list)!= NULL)&& ret==NULL){
			printf("%d\n",num);
			ret = strstr(temp, tempp);
			num++;
			if(ret !=NULL){

			}

		}
	}
	
	fclose(list);
	
	
	if (ret == NULL) printf("LOL\n" );*/
	return 1;
}