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

#define HTTP_PORT 80
#define SIZE_BUFF 1024
#define KO 1024
#define SERV_PORT 12000
// proxyHTTP
/*
	Prototypes
*/
int proxy(int socket);
char * get_host(char * requete);


int test_pub(char * host){
	FILE * list= NULL;
	list = fopen("easylist-downloads.adblockplus.org/easylist.txt","r");
	char *ret= NULL;
	int res = 0;
	char temp[300];
	if(list != NULL){
		// si ret == NULL alors il ne contient pas le mot dans la chaine
		while((fgets(temp,300,list)!= NULL)&& ret==NULL){
			ret = strstr(temp, host);
		}
	}
	else{
		printf("erreur sur la easylist.txt fichier absent\n");
	}
	if(ret!= NULL){
		res =1;
	} 
	fclose(list);
	return res;
}

char * get_host(char * requete){

	unsigned short i = 0, j= 0 ;
	char * buffer = (char * )malloc(2*1024 * sizeof(char));
	if(!(buffer = strstr(requete, "Host: " )) ){
		printf("erreur sur la requete impossible de recuperer l'host\n");
		return "err";
	}
	else{
		char * host = (char *)malloc(256 * sizeof(char));
		while( buffer[i] != '\n' ){
			i++;
		}
		for(j = 6; j < i-1; j++){
			//printf("%c",(host[j-6]) );
			host[j - 6] = buffer[j];
		}
		//printf("\n");
		host[j-6+1] = '\0';
		return host;
	}
	

}
/*
 Fonction proxy l'envoi puis la reception de la requete du navigateur
 return 0 si ok
 sinon -1
*/
int proxy(int sock){
	int pub = 0;
	int * tmp = &sock;
	int socket_client = *tmp;
	struct hostent* host;
	int get;
	char* temp=NULL;
	int socket_send;
	char site[40];
	struct sockaddr_in send_add;
	struct addrinfo * add_serv;
	char buffR[SIZE_BUFF]; // buffer de 2Ko
	char buffR1[SIZE_BUFF]; // buffer de 4Ko
	char t1[600],t2[600],t3[20];
	//vidange des buffer
	bzero((char*)buffR,sizeof(buffR));
	bzero((char*)buffR1,sizeof(buffR1));
	//memset(buffR,0, sizeof(buffR));
	//memset(buffR1,0, sizeof(buffR1));
	int size = recv(socket_client,buffR,SIZE_BUFF,0);
	buffR[size]=0;
	// on recupere l'URL
	// recuperation des information d'entete
	sscanf(buffR,"%s %s %s",t1,t2,t3);
	if( !((strncmp(t1,"GET",3)==0) 
		&& ((strncmp(t3,"HTTP/1.1",8)==0)||(strncmp(t3,"HTTP/1.0",8)==0)) 
		&&  (strncmp(t2,"http://",7)==0) ) ){

		printf("erreur sur la requete HTTP  invalide\n");
		return -1;
	}
	strcpy(t1,t2);
	// on recupere http:
	temp= strtok(t2,"//");
	// on recupere l'HOST
	temp=strtok(NULL,"/");
	strcpy(site,temp);
	// on met l'HOST dans t2
	sprintf(t2,"%s",temp);
	host=gethostbyname(t2);


	strcat(t1,"^]");
	temp = strtok(t1,"//");
	temp = strtok(NULL, "/");
	// on recupere le chemin apres nom d'host
	if(temp!= NULL)
		temp = strtok(NULL,"^]");
	
	// si c'est une pub on envoi pas de requete au serveur
	if(pub ==0){
		bzero((char*)&send_add,sizeof(send_add));
		send_add.sin_family = AF_INET;
		send_add.sin_port = htons(HTTP_PORT);
		//send_add.sin_addr = *addr;
		bcopy((char*)host->h_addr,(char*)&send_add.sin_addr.s_addr,host->h_length);
		//memset(&(send_add.sin_zero),'\0',8);
		
		if( (socket_send = socket(AF_INET, SOCK_STREAM, 0)) < 0){
			printf("Erreur sur la creation de la socket de com vers domaine\n");
			close(socket_client);
			return -1;
		}
		if(connect(socket_send,(struct sockaddr * )&send_add, sizeof(send_add))<0 ){
			printf("Erreur sur la mise en place de la connexion avec le domaine %d\n", size);
			close(socket_client);
			close(socket_send);
			return -1;
		}
		bzero((char*)buffR,sizeof(buffR));
		if(temp!=NULL)
			sprintf(buffR,"GET /%s %s\r\nHost: %s\r\nConnection: close\r\n\r\n",temp,t3,t2);
		else
			sprintf(buffR,"GET / %s\r\nHost: %s\r\nConnection: close\r\n\r\n",t3,t2);

		// teste si c'est de la pub 

		pub = test_pub(site);
		//printf("PUB %d\n", pub);
		//envoi de la requete au serveur
		
		if(send(socket_send,buffR, (int)strlen(buffR),0) < 0){
			printf("Erreur sur l'envoi vers le serveur\n");
			close(socket_client);
			close(socket_send);
			return -1;
		}
		
		
	// on attend la reponce du domaine
	// puis transfere vers le navigateur

	
		while( (size = recv(socket_send,buffR1,SIZE_BUFF,0)) >0 ){

			// on renvoie la reponse avec \0 pour marquer la fin
			
			if( send(socket_client,buffR1, size,0)< 0){
				printf("Erreur sur l'envoi vers le client\n");
				close(socket_client);
				close(socket_send);
				return -1;
			}
			bzero((char *)buffR1,SIZE_BUFF);
		}
	}
	else{
		if(send(socket_send,"404 : NOT FOUND\nONLY HTTP REQUESTS ALLOWED",18,0)< 0 ){
			printf("Erreur sur l'envoi vers le serveur PUB\n");
			close(socket_client);
			close(socket_send);
		return -1;
		}
	}
	shutdown(socket_client,2);
	shutdown(socket_send,2);
	close(socket_client);
	close(socket_send);

	//printf("Requete executé\n" );

	return 0;
}

int main ( void )
{
	printf("\n********* MyAdBlock ***********\n");
	printf("\n-- M.Toussaint & J-E.Schouver --\n");
	printf("\n******** Port : 12000 *********\n\n");
	

	int socket_server;
	int socket_client;
	struct sockaddr_in srv_add;
	struct sockaddr_in cli_add;
	int size_client , lg;

	bzero((char*)&srv_add,sizeof(srv_add));
	bzero((char*)&cli_add, sizeof(cli_add));

	size_client = sizeof(cli_add);
	//on remplit l'espace alloué a srv_add avec des 0
	//memset(&srv_add,0,sizeof(srv_add));

	//initialisation socket d'ecoute pour navigateur
	// INADDR_ANY on recupere l'@IP de la machine en dynamic
	srv_add.sin_port = htons(SERV_PORT);
	srv_add.sin_family = AF_INET;
	srv_add.sin_addr.s_addr = htonl(INADDR_ANY);
	//printf("%s\n",srv_add.sin_addr.s_addr);
	/* creation de socket */
	if ((socket_server = socket(AF_INET,SOCK_STREAM,0))< 0)
	{
	  perror("erreur socket\n");
	  exit(1);
	}
	/* bind serveur - socket */
	if(bind(socket_server,(struct sockaddr *)&srv_add,sizeof(srv_add)) < 0){
			perror("erreur bind\n");
			exit(1);
		}
	/* ecoute sur la socket */
	if(listen(socket_server,SOMAXCONN)<0){
		perror("erreur ecoute\n");
		exit(1);
	}
	/* accept la connexion */

		while(1){
			socket_client = accept(socket_server,(struct sockaddr *)&cli_add,(socklen_t *)&size_client);
			if(socket_client<0){
				perror("erreur accept\n");
				exit(1);
			}
			if (fork() == 0){
				//on est dans le fils
				

				//shutdown(socket_client,2);
				
				//appel de la fonction de service proxy
				proxy(socket_client);
				close(socket_server);
				//close(socket_client);
				exit(0);
			}
		}
		shutdown(socket_server,2);
		close(socket_server);
		exit(0);
}